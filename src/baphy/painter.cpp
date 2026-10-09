#include "baphy/painter.hpp"

#include <array>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <tuple>
#include "baphy/detail/shaders.hpp"
#include "baphy/util/math.hpp"

static constexpr std::size_t batch_size(
        const std::size_t vertex_size_bytes, const std::size_t vertex_count, const std::size_t max_batch_size_bytes) {
    return vertex_count * (max_batch_size_bytes / (vertex_size_bytes * vertex_count));
}

static_assert(sizeof(baphy::PrimitiveVertex) == 16);
static_assert(sizeof(baphy::TextureVertex) == 24);
constexpr std::size_t PRIMITIVE_BATCH_SIZE = batch_size(sizeof(baphy::PrimitiveVertex), 3, 1024 * 1024);
constexpr std::size_t TEXTURE_BATCH_SIZE = batch_size(sizeof(baphy::TextureVertex), 6, 1024 * 1024);

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

    opaq_primitive_vbos_ =
            std::make_unique<detail::VBOList<PrimitiveVertex>>(PRIMITIVE_BATCH_SIZE, glh::FillDirection::Reverse);

    trans_primitive_vbos_ =
            std::make_unique<detail::VBOList<PrimitiveVertex>>(PRIMITIVE_BATCH_SIZE, glh::FillDirection::Forward);

    tex_vbos_ = std::make_unique<detail::VBOList<TextureVertex>>(PRIMITIVE_BATCH_SIZE, glh::FillDirection::Forward);
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

    fill_tri(a, c, d, color);
    fill_tri(a, d, b, color);
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
        fill_tri_translucent_(p0, p1, p2, c);
    else
        fill_tri_opaque_(p0, p1, p2, c);
}

void baphy::Painter::fill_rect(const glm::vec2 p0, const glm::vec2 size, const Color &color) {
    fill_tri(p0, p0 + glm::vec2{size.x, 0.0f}, p0 + size, color);
    fill_tri(p0, p0 + size, p0 + glm::vec2{0.0f, size.y}, color);
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

void baphy::Painter::tex(const Texture &t, const glm::vec2 p0, const glm::vec2 size, const Color &color) {
    if (!tex_vbos_->can_fit(6)) {
        save_draw_call_();
        tex_vbos_->advance();
    }

    start_draw_call_(t.id());

    const auto z = next_z_(GeometryType::translucent);
    const auto c = to_vertex_color(color);
    const auto p1 = p0 + glm::vec2{size.x, 0.0f};
    const auto p2 = p0 + size;
    const auto p3 = p0 + glm::vec2{0.0f, size.y};

    tex_vbos_->extend(std::array{
            TextureVertex{{p0.x, p0.y, z}, c, {0.0f, 0.0f}},
            TextureVertex{{p1.x, p1.y, z}, c, {1.0f, 0.0f}},
            TextureVertex{{p2.x, p2.y, z}, c, {1.0f, 1.0f}},
            TextureVertex{{p0.x, p0.y, z}, c, {0.0f, 0.0f}},
            TextureVertex{{p2.x, p2.y, z}, c, {1.0f, 1.0f}},
            TextureVertex{{p3.x, p3.y, z}, c, {0.0f, 1.0f}},
    });
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
    for (const auto &vbo: *opaq_primitive_vbos_ | std::views::reverse) {
        glVertexArrayVertexBuffer(primitive_vao_, 0, vbo->id, 0, sizeof(PrimitiveVertex));
        glDrawArrays(GL_TRIANGLES, static_cast<GLint>(vbo->front()), static_cast<GLsizei>(vbo->size()));
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

    for (const auto &[tex_id, vbo_id, first, count]: trans_draw_calls_) {
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
            bound_vbo_id = vbo_id;
        }

        if (is_tex && bound_tex_id != tex_id) {
            glBindTextureUnit(0, *tex_id);
            bound_tex_id = tex_id;
        }

        glDrawArrays(GL_TRIANGLES, first, count);
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

void baphy::Painter::fill_tri_opaque_(
        const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, const glm::u8vec4 color) {
    if (!opaq_primitive_vbos_->can_fit(3))
        opaq_primitive_vbos_->advance();

    const auto z = next_z_(GeometryType::opaque);
    opaq_primitive_vbos_->extend(std::array{
            PrimitiveVertex{{p0.x, p0.y, z}, color},
            PrimitiveVertex{{p1.x, p1.y, z}, color},
            PrimitiveVertex{{p2.x, p2.y, z}, color},
    });
}

void baphy::Painter::fill_tri_translucent_(
        const glm::vec2 p0, const glm::vec2 p1, const glm::vec2 p2, const glm::u8vec4 color) {
    if (!trans_primitive_vbos_->can_fit(3)) {
        save_draw_call_();
        trans_primitive_vbos_->advance();
    }

    start_draw_call_(std::nullopt);

    const auto z = next_z_(GeometryType::translucent);
    trans_primitive_vbos_->extend(std::array{
            PrimitiveVertex{{p0.x, p0.y, z}, color},
            PrimitiveVertex{{p1.x, p1.y, z}, color},
            PrimitiveVertex{{p2.x, p2.y, z}, color},
    });
}

std::pair<GLuint, std::size_t> baphy::Painter::curr_translucent_vbo_(const bool tex) const {
    if (tex)
        return {tex_vbos_->id(), tex_vbos_->back()};
    return {trans_primitive_vbos_->id(), trans_primitive_vbos_->back()};
}

void baphy::Painter::start_draw_call_(const std::optional<GLuint> tex_id) {
    if (pending_trans_draw_call_ && pending_trans_draw_call_->tex_id != tex_id)
        save_draw_call_();

    if (!pending_trans_draw_call_) {
        const auto [_, back] = curr_translucent_vbo_(tex_id.has_value());
        pending_trans_draw_call_ = PendingDrawCall{tex_id, back};
    }
}

void baphy::Painter::save_draw_call_() {
    if (!pending_trans_draw_call_)
        return;

    const auto [tex_id, first] = *pending_trans_draw_call_;
    pending_trans_draw_call_.reset();

    const auto [vbo_id, back] = curr_translucent_vbo_(tex_id.has_value());
    if (back > first)
        trans_draw_calls_.emplace_back(tex_id, vbo_id, static_cast<GLint>(first), static_cast<GLsizei>(back - first));
}
