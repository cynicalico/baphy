#include "baphy/painter.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <tuple>
#include "baphy/detail/shaders.hpp"
#include "baphy/util/math.hpp"

using Index = baphy::detail::VBOList<baphy::PrimitiveVertex>::index_t;
static_assert(std::same_as<Index, baphy::detail::VBOList<baphy::TextureVertex>::index_t>);
constexpr GLenum INDEX_GL_TYPE = baphy::detail::VBOList<baphy::PrimitiveVertex>::index_gl_type;

static constexpr std::size_t
vertex_batch_size(const std::size_t vertex_size_bytes, const std::size_t max_batch_size_bytes) {
    return std::min(max_batch_size_bytes / vertex_size_bytes,
                    baphy::detail::VBOList<baphy::PrimitiveVertex>::max_vertex_capacity);
}

static constexpr std::size_t index_batch_size(const std::size_t vertex_batch_size) {
    return vertex_batch_size / 4 * 6;
}

static_assert(sizeof(baphy::PrimitiveVertex) == 16);
static_assert(sizeof(baphy::TextureVertex) == 24);
constexpr std::size_t MAX_BATCH_BYTES = 2 * 1024 * 1024;
constexpr std::size_t PRIMITIVE_BATCH_SIZE = vertex_batch_size(sizeof(baphy::PrimitiveVertex), MAX_BATCH_BYTES);
constexpr std::size_t TEXTURE_BATCH_SIZE = vertex_batch_size(sizeof(baphy::TextureVertex), MAX_BATCH_BYTES);

constexpr std::array<Index, 3> TRI_INDICES{0, 1, 2};
constexpr std::array<Index, 6> QUAD_INDICES{0, 1, 2, 0, 2, 3};

static glm::u8vec4 to_vertex_color(const baphy::Color &color) {
    return {glm::round(glm::clamp(color.value(), 0.0f, 1.0f) * 255.0f)};
}

baphy::Painter::Painter() {
    primitive_shader_ = glh::create_shader_from_src(detail::PRIMITIVE_VERT_SRC, detail::PRIMITIVE_FRAG_SRC).value();
    tex_shader_ = glh::create_shader_from_src(detail::TEXTURE_VERT_SRC, detail::TEXTURE_FRAG_SRC).value();
    uniform_loc_cache_ = std::make_unique<detail::ShaderUniformLocCache>();

    primitive_vao_ = glh::create_vertex_array_from_bindings(
            primitive_shader_,
            {{"aPos", 3, GL_FLOAT, false, offsetof(PrimitiveVertex, pos)},
             {"aColor", 4, GL_UNSIGNED_BYTE, true, offsetof(PrimitiveVertex, color)}});

    tex_vao_ = glh::create_vertex_array_from_bindings(
            tex_shader_,
            {{"aPos", 3, GL_FLOAT, false, offsetof(TextureVertex, pos)},
             {"aColor", 4, GL_UNSIGNED_BYTE, true, offsetof(TextureVertex, color)},
             {"aTexCoord", 2, GL_FLOAT, false, offsetof(TextureVertex, tex_coord)}});

    opaq_primitive_vbos_ = std::make_unique<detail::VBOList<PrimitiveVertex>>(
            PRIMITIVE_BATCH_SIZE, index_batch_size(PRIMITIVE_BATCH_SIZE), glh::FillDirection::Reverse);

    trans_primitive_vbos_ = std::make_unique<detail::VBOList<PrimitiveVertex>>(
            PRIMITIVE_BATCH_SIZE, index_batch_size(PRIMITIVE_BATCH_SIZE), glh::FillDirection::Forward);

    tex_vbos_ = std::make_unique<detail::VBOList<TextureVertex>>(
            TEXTURE_BATCH_SIZE, index_batch_size(TEXTURE_BATCH_SIZE), glh::FillDirection::Forward);
}

baphy::Painter::~Painter() {
    glDeleteVertexArrays(1, &tex_vao_);
    glDeleteProgram(tex_shader_);

    glDeleteVertexArrays(1, &primitive_vao_);
    glDeleteProgram(primitive_shader_);
}

void baphy::Painter::reset() {
    z_ = 1.0f;
    last_geometry_type_ = std::nullopt;

    opaq_primitive_vbos_->clear();
    trans_primitive_vbos_->clear();
    tex_vbos_->clear();

    pending_trans_draw_call_ = std::nullopt;
    trans_draw_calls_.clear();
}

void baphy::Painter::point(glm::vec2 p0, const Color &color) {
    fill_square(p0, 1.0f, color);
}

void baphy::Painter::line(const glm::vec2 p0, const glm::vec2 p1, const float width, const Color &color) {
    const auto half_w = width / 2.0f;
    const auto offset = perp(glm::normalize(p1 - p0)) * half_w;

    const auto a = p0 + offset;
    const auto b = p0 - offset;
    const auto c = p1 + offset;
    const auto d = p1 - offset;

    fill_quad_(a, c, d, b, color);
}

void baphy::Painter::polyline(
        std::span<const glm::vec2> points, const float width, bool closed, const LineJoin join, const Color &color) {
    std::vector<std::array<glm::vec2, 3>> tris;
    detail::triangulate_polyline(points, width, closed, join, tris);
    for (const auto &t: tris)
        fill_tri(t[0], t[1], t[2], color);
}

void baphy::Painter::fill_tri(const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, const Color &color) {
    const auto c = to_vertex_color(color);
    if (c.a < 255)
        fill_poly_translucent_(std::array{p0, p1, p2}, TRI_INDICES, c);
    else
        fill_poly_opaque_(std::array{p0, p1, p2}, TRI_INDICES, c);
}

void baphy::Painter::fill_rect(const glm::vec2 p0, const glm::vec2 size, const Color &color) {
    fill_quad_(p0, p0 + glm::vec2{size.x, 0.0f}, p0 + size, p0 + glm::vec2{0.0f, size.y}, color);
}

void baphy::Painter::fill_square(const glm::vec2 p0, const float size, const Color &color) {
    fill_rect(p0, {size, size}, color);
}

void baphy::Painter::fill_ellipse(glm::vec2 center, glm::vec2 size, const Color &color) {
    constexpr auto t0 = glm::radians(0.0f);
    const auto p0 = point_on_ellipse(center, size, t0);

    constexpr auto t1 = glm::radians(120.0f);
    const auto p1 = point_on_ellipse(center, size, t1);

    constexpr auto t2 = glm::radians(240.0f);
    const auto p2 = point_on_ellipse(center, size, t2);

    fill_tri(p0, p1, p2, color);

    // we need this because the midpoint calculation won't work otherwise
    constexpr auto t3 = glm::radians(360.0f);
    const auto p3 = point_on_ellipse(center, size, t3);

    auto base_points = std::stack<std::tuple<glm::vec2, float, glm::vec2, float>>();
    base_points.emplace(p0, t0, p1, t1);
    base_points.emplace(p1, t1, p2, t2);
    base_points.emplace(p2, t2, p3, t3);

    while (!base_points.empty()) {
        const auto [pa, ta, pb, tb] = base_points.top();
        base_points.pop();

        const auto tm = (ta + tb) / 2.0f;
        const auto pm = point_on_ellipse(center, size, tm);

        const auto mid = (pa + pb) / 2.0f;
        const auto dist2 = glm::dot(pm - mid, pm - mid);
        // skip any vectors with a length less than 0.5
        if (dist2 >= 0.5 * 0.5) {
            fill_tri(pa, pm, pb, color);

            base_points.emplace(pa, ta, pm, tm);
            base_points.emplace(pm, tm, pb, tb);
        }
    }
}

void baphy::Painter::fill_circle(glm::vec2 center, float size, const Color &color) {
    fill_ellipse(center, {size, size}, color);
}

void baphy::Painter::stroke_tri(
        const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, const float line_width, const Color &color) {
    const auto hw = line_width / 2.0f;

    const auto a = glm::length(p1 - p2);
    const auto b = glm::length(p0 - p2);
    const auto c = glm::length(p0 - p1);
    const auto perimeter = a + b + c;

    const auto e0 = p1 - p0;
    const auto e1 = p2 - p0;
    const auto area = std::abs(e0.x * e1.y - e0.y * e1.x) / 2.0f;
    const auto inradius = perimeter > 0.0f ? 2.0f * area / perimeter : 0.0f;

    // stroke would cover the whole triangle
    if (inradius <= hw) {
        fill_tri(p0, p1, p2, color);
        return;
    }

    const auto incenter = (a * p0 + b * p1 + c * p2) / perimeter;
    const auto scale = (inradius - hw) / inradius;
    polyline(std::array{incenter + (p0 - incenter) * scale,
                        incenter + (p1 - incenter) * scale,
                        incenter + (p2 - incenter) * scale},
             line_width,
             true,
             LineJoin::miter,
             color);
}

void baphy::Painter::stroke_rect(glm::vec2 p0, glm::vec2 size, float line_width, const Color &color) {
    // stroke would cover the whole rect
    if (size.x <= line_width || size.y <= line_width) {
        fill_rect(p0, size, color);
        return;
    }

    const auto hw = line_width / 2.0f;
    const auto tl = p0 + hw;
    const auto br = p0 + size - hw;
    polyline(
            std::array{tl, glm::vec2{br.x, tl.y}, br, glm::vec2{tl.x, br.y}}, line_width, true, LineJoin::miter, color);
}

void baphy::Painter::stroke_square(const glm::vec2 p0, const float size, const float line_width, const Color &color) {
    stroke_rect(p0, {size, size}, line_width, color);
}

void baphy::Painter::stroke_ellipse(
        const glm::vec2 center, const glm::vec2 size, const float line_width, const Color &color) {
    const auto hw = line_width / 2.0f;

    // stroke would cover the whole ellipse
    if (size.x <= hw || size.y <= hw) {
        fill_ellipse(center, size, color);
        return;
    }

    const auto path_size = size - hw;

    constexpr auto t0 = glm::radians(0.0f);
    const auto p0 = point_on_ellipse(center, path_size, t0);

    constexpr auto t1 = glm::radians(120.0f);
    const auto p1 = point_on_ellipse(center, path_size, t1);

    constexpr auto t2 = glm::radians(240.0f);
    const auto p2 = point_on_ellipse(center, path_size, t2);

    // we need this because the midpoint calculation won't work otherwise
    constexpr auto t3 = glm::radians(360.0f);
    const auto p3 = point_on_ellipse(center, path_size, t3);

    // p3 is the same point as p0, so it isn't added
    auto edge_points = std::vector<std::tuple<float, glm::vec2>>{{t0, p0}, {t1, p1}, {t2, p2}};

    auto base_points = std::stack<std::tuple<glm::vec2, float, glm::vec2, float>>();
    base_points.emplace(p0, t0, p1, t1);
    base_points.emplace(p1, t1, p2, t2);
    base_points.emplace(p2, t2, p3, t3);

    while (!base_points.empty()) {
        const auto [pa, ta, pb, tb] = base_points.top();
        base_points.pop();

        const auto tm = (ta + tb) / 2.0f;
        const auto pm = point_on_ellipse(center, path_size, tm);

        const auto mid = (pa + pb) / 2.0f;
        const auto dist2 = glm::dot(pm - mid, pm - mid);
        // skip any vectors with a length less than 0.5
        if (dist2 >= 0.5 * 0.5) {
            edge_points.emplace_back(tm, pm);

            base_points.emplace(pa, ta, pm, tm);
            base_points.emplace(pm, tm, pb, tb);
        }
    }

    std::ranges::sort(edge_points, {}, [](const auto &e) {
        return std::get<0>(e);
    });

    auto points = std::vector<glm::vec2>();
    points.reserve(edge_points.size());
    for (const auto &[t, p]: edge_points)
        points.push_back(p);

    polyline(points, line_width, true, LineJoin::miter, color);
}

void baphy::Painter::stroke_circle(
        const glm::vec2 center, const float size, const float line_width, const Color &color) {
    stroke_ellipse(center, {size, size}, line_width, color);
}

void baphy::Painter::tex(
        const Texture &t, const glm::vec2 p0, const std::optional<glm::vec2> size, const Color &color) {
    tex_sub(t, p0, size.value_or(t.size_f()), {0.0f, 0.0f}, t.size_f(), color);
}

void baphy::Painter::tex_sub(const Texture &t,
                             glm::vec2 p0,
                             std::optional<glm::vec2> size,
                             glm::vec2 sub_p0,
                             glm::vec2 sub_size,
                             const Color &color) {
    if (!tex_vbos_->can_fit(4, QUAD_INDICES.size())) {
        save_draw_call_();
        tex_vbos_->advance();
    }

    start_draw_call_(t.id());

    const auto z = next_z_(GeometryType::translucent);
    const auto c = to_vertex_color(color);
    const auto s = size.value_or(sub_size);

    const auto p1 = p0 + glm::vec2{s.x, 0.0f};
    const auto p2 = p0 + s;
    const auto p3 = p0 + glm::vec2{0.0f, s.y};

    const auto t0 = t.to_tex_coords(sub_p0);
    const auto t1 = t.to_tex_coords(sub_p0 + glm::vec2(sub_size.x, 0.0f));
    const auto t2 = t.to_tex_coords(sub_p0 + sub_size);
    const auto t3 = t.to_tex_coords(sub_p0 + glm::vec2(0.0f, sub_size.y));

    tex_vbos_->extend(
            std::array{
                    TextureVertex{{p0.x, p0.y, z}, c, t0},
                    TextureVertex{{p1.x, p1.y, z}, c, t1},
                    TextureVertex{{p2.x, p2.y, z}, c, t2},
                    TextureVertex{{p3.x, p3.y, z}, c, t3},
            },
            QUAD_INDICES);
}

void baphy::Painter::draw(const glm::mat4 &projection) {
    opaq_primitive_vbos_->sync();
    trans_primitive_vbos_->sync();
    tex_vbos_->sync();

    const auto z_max = z_ + 1.0f;

    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);

    draw_opaque_(projection, z_max);
    draw_translucent_(projection, z_max);

    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glClipControl(GL_LOWER_LEFT, GL_NEGATIVE_ONE_TO_ONE);

    reset();
}

void baphy::Painter::draw_opaque_(const glm::mat4 &projection, float z_max) {
    glProgramUniformMatrix4fv(primitive_shader_,
                              uniform_loc_cache_->find(primitive_shader_, "projection"),
                              1,
                              GL_FALSE,
                              glm::value_ptr(projection));
    glProgramUniform1f(primitive_shader_, uniform_loc_cache_->find(primitive_shader_, "z_max"), z_max);

    glUseProgram(primitive_shader_);
    glBindVertexArray(primitive_vao_);
    for (const auto &batch: *opaq_primitive_vbos_ | std::views::reverse) {
        glVertexArrayVertexBuffer(primitive_vao_, 0, batch->vertices.id, 0, sizeof(PrimitiveVertex));
        glVertexArrayElementBuffer(primitive_vao_, batch->indices.id);
        glDrawElements(GL_TRIANGLES,
                       static_cast<GLsizei>(batch->indices.size()),
                       INDEX_GL_TYPE,
                       reinterpret_cast<const void *>(batch->indices.front() * sizeof(Index)));
    }
}

void baphy::Painter::draw_translucent_(const glm::mat4 &projection, float z_max) {
    save_draw_call_();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glProgramUniformMatrix4fv(
            tex_shader_, uniform_loc_cache_->find(tex_shader_, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glProgramUniform1f(tex_shader_, uniform_loc_cache_->find(tex_shader_, "z_max"), z_max);
    glProgramUniform1i(tex_shader_, uniform_loc_cache_->find(tex_shader_, "tex"), 0);

    std::optional<bool> bound_tex{std::nullopt};
    std::optional<GLuint> bound_vbo_id{std::nullopt};
    std::optional<GLuint> bound_tex_id{std::nullopt};

    for (const auto &[tex_id, vbo_id, ebo_id, first, count]: trans_draw_calls_) {
        const auto is_tex = tex_id.has_value();
        const auto vao = is_tex ? tex_vao_ : primitive_vao_;

        if (bound_tex != is_tex) {
            glUseProgram(is_tex ? tex_shader_ : primitive_shader_);
            glBindVertexArray(vao);
            bound_tex = is_tex;
            bound_vbo_id = std::nullopt;
        }

        if (bound_vbo_id != vbo_id) {
            glVertexArrayVertexBuffer(vao, 0, vbo_id, 0, is_tex ? sizeof(TextureVertex) : sizeof(PrimitiveVertex));
            glVertexArrayElementBuffer(vao, ebo_id);
            bound_vbo_id = vbo_id;
        }

        if (is_tex && bound_tex_id != tex_id) {
            glBindTextureUnit(0, *tex_id);
            bound_tex_id = tex_id;
        }

        glDrawElements(GL_TRIANGLES, count, INDEX_GL_TYPE, reinterpret_cast<const void *>(first * sizeof(Index)));
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

float baphy::Painter::next_z_(const GeometryType type) {
    if (last_geometry_type_ && *last_geometry_type_ != type)
        z_ += 1.0f;
    last_geometry_type_ = type;
    return z_;
}

void baphy::Painter::fill_quad_(
        const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, const glm::vec2 p3, const Color &color) {
    if (const auto c = to_vertex_color(color); c.a < 255)
        fill_poly_translucent_(std::array{p0, p1, p2, p3}, QUAD_INDICES, c);
    else
        fill_poly_opaque_(std::array{p0, p1, p2, p3}, QUAD_INDICES, c);
}

template<std::size_t N, std::size_t M>
void baphy::Painter::fill_poly_opaque_(
        const std::array<glm::vec2, N> &points, const std::array<Index, M> &indices, const glm::u8vec4 color) {
    if (!opaq_primitive_vbos_->can_fit(N, M))
        opaq_primitive_vbos_->advance();

    const auto z = next_z_(GeometryType::opaque);
    std::array<PrimitiveVertex, N> vertices;
    for (std::size_t i = 0; i < N; ++i)
        vertices[i] = {{points[i].x, points[i].y, z}, color};

    opaq_primitive_vbos_->extend(vertices, indices);
}

template<std::size_t N, std::size_t M>
void baphy::Painter::fill_poly_translucent_(
        const std::array<glm::vec2, N> &points, const std::array<Index, M> &indices, const glm::u8vec4 color) {
    if (!trans_primitive_vbos_->can_fit(N, M)) {
        save_draw_call_();
        trans_primitive_vbos_->advance();
    }

    start_draw_call_(std::nullopt);

    const auto z = next_z_(GeometryType::translucent);
    std::array<PrimitiveVertex, N> vertices;
    for (std::size_t i = 0; i < N; ++i)
        vertices[i] = {{points[i].x, points[i].y, z}, color};

    trans_primitive_vbos_->extend(vertices, indices);
}

baphy::Painter::CurrentBatch baphy::Painter::curr_translucent_batch_(const bool tex) const {
    if (tex)
        return {tex_vbos_->vbo_id(), tex_vbos_->ebo_id(), tex_vbos_->index_back()};
    return {trans_primitive_vbos_->vbo_id(), trans_primitive_vbos_->ebo_id(), trans_primitive_vbos_->index_back()};
}

void baphy::Painter::start_draw_call_(const std::optional<GLuint> tex_id) {
    if (pending_trans_draw_call_ && pending_trans_draw_call_->tex_id != tex_id)
        save_draw_call_();

    if (!pending_trans_draw_call_)
        pending_trans_draw_call_ = PendingDrawCall{tex_id, curr_translucent_batch_(tex_id.has_value()).index_back};
}

void baphy::Painter::save_draw_call_() {
    if (!pending_trans_draw_call_)
        return;

    const auto [tex_id, first] = *pending_trans_draw_call_;
    pending_trans_draw_call_.reset();

    const auto [vbo_id, ebo_id, back] = curr_translucent_batch_(tex_id.has_value());
    if (back > first)
        trans_draw_calls_.emplace_back(tex_id, vbo_id, ebo_id, first, static_cast<GLsizei>(back - first));
}
