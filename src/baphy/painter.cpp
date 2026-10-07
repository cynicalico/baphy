#include "baphy/painter.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <numbers>
#include <stack>
#include <tuple>
#include "baphy/util/math.hpp"

constexpr auto PRIMITIVE_VERT_SRC = R"glsl(
#version 460 core

in vec3 aPos;
in vec3 aColor;

out vec3 frag_color;

uniform mat4 projection;

void main() {
    frag_color = aColor;

    gl_Position = projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)glsl";

constexpr auto PRIMITIVE_FRAG_SRC = R"glsl(
#version 460 core

out vec4 FragColor;

in vec3 frag_color;

void main() {
    FragColor = vec4(frag_color, 1.0f);
}
)glsl";

constexpr std::size_t batch_size(const std::size_t vertex_size_bytes,
                                 const std::size_t vertex_count,
                                 const std::size_t max_batch_size_bytes) {
  return vertex_count *
         (max_batch_size_bytes / (vertex_size_bytes * vertex_count));
}

constexpr std::size_t PRIMITIVE_BATCH_SIZE =
    batch_size(sizeof(baphy::PrimitiveVertex), 3, 1024 * 1024);

baphy::Painter::Painter() {
  primitive_shader_ =
      glh::create_shader_from_src(PRIMITIVE_VERT_SRC, PRIMITIVE_FRAG_SRC)
          .value();

  primitive_vao_ = glh::create_vertex_array_from_bindings(
      primitive_shader_,
      {{"aPos", 3, GL_FLOAT, false, offsetof(PrimitiveVertex, pos)},
       {"aColor", 3, GL_FLOAT, false, offsetof(PrimitiveVertex, color)}});

  primitive_vbos_.emplace_back(
      std::make_unique<glh::VecBuffer<PrimitiveVertex>>(PRIMITIVE_BATCH_SIZE));
}

baphy::Painter::~Painter() {
  primitive_vbos_.clear();
  glDeleteVertexArrays(1, &primitive_vao_);
  glDeleteProgram(primitive_shader_);
}

void baphy::Painter::point(glm::vec2 p0, const Color &color) {
  fill_square(p0, 1.0f, color);
}

void baphy::Painter::line(const glm::vec2 p0,
                          const glm::vec2 p1,
                          const float width,
                          const Color &color) {
  const auto half_w = width / 2.0f;
  const auto offset = perp(glm::normalize(p1 - p0)) * half_w;

  const auto a = p0 + offset;
  const auto b = p0 - offset;
  const auto c = p1 + offset;
  const auto d = p1 - offset;

  fill_tri(a, c, d, color);
  fill_tri(a, d, b, color);
}

void baphy::Painter::polyline(std::span<const glm::vec2> points,
                              const float width,
                              bool closed,
                              const LineJoin join,
                              const Color &color) {
  std::vector<std::array<glm::vec2, 3>> tris;
  detail::triangulate_polyline(points, width, closed, join, tris);
  for (const auto &t: tris)
    fill_tri(t[0], t[1], t[2], color);
}

void baphy::Painter::fill_tri(const glm::vec2 p0,
                              const glm::vec2 p1,
                              const glm::vec2 p2,
                              const Color &color) {
  if (!primitive_vbos_[curr_primitive_vbo_idx_]->can_fit(3)) {
    curr_primitive_vbo_idx_++;
    if (primitive_vbos_.size() <= curr_primitive_vbo_idx_) {
      primitive_vbos_.emplace_back(
          std::make_unique<glh::VecBuffer<PrimitiveVertex>>(
              PRIMITIVE_BATCH_SIZE, glh::FillDirection::Reverse));
    }
  }

  const auto gl_color = color.value();
  primitive_vbos_[curr_primitive_vbo_idx_]->extend(std::array{
      PrimitiveVertex{{p0.x, p0.y, 0.0f}, {gl_color.r, gl_color.g, gl_color.b}},
      PrimitiveVertex{{p1.x, p1.y, 0.0f}, {gl_color.r, gl_color.g, gl_color.b}},
      PrimitiveVertex{{p2.x, p2.y, 0.0f}, {gl_color.r, gl_color.g, gl_color.b}},
  });
}

void baphy::Painter::fill_rect(const glm::vec2 p0,
                               const glm::vec2 size,
                               const Color &color) {
  fill_tri(p0, p0 + glm::vec2{size.x, 0.0f}, p0 + size, color);
  fill_tri(p0, p0 + size, p0 + glm::vec2{0.0f, size.y}, color);
}

void baphy::Painter::fill_square(
    const glm::vec2 p0, const float size, const Color &color) {
  fill_rect(p0, {size, size}, color);
}

void baphy::Painter::fill_ellipse(
    glm::vec2 center, glm::vec2 size, const Color &color) {
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

  auto base_points =
      std::stack<std::tuple<glm::vec2, float, glm::vec2, float>>();
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

void baphy::Painter::fill_circle(
    glm::vec2 center, float size, const Color &color) {
  fill_ellipse(center, {size, size}, color);
}

void baphy::Painter::draw(const glm::mat4 &projection) {
  glDepthFunc(GL_GREATER);

  for (const auto &vbo: primitive_vbos_)
    vbo->sync();

  glUseProgram(primitive_shader_);
  glUniformMatrix4fv(glGetUniformLocation(primitive_shader_, "projection"),
                     1,
                     GL_FALSE,
                     glm::value_ptr(projection));

  glBindVertexArray(primitive_vao_);
  for (const auto &vbo: primitive_vbos_ | std::views::reverse) {
    glVertexArrayVertexBuffer(
        primitive_vao_, 0, vbo->id, 0, sizeof(PrimitiveVertex));
    glDrawArrays(GL_TRIANGLES,
                 static_cast<GLint>(vbo->front()),
                 static_cast<GLsizei>(vbo->size()));
  }

  glDepthFunc(GL_LESS);

  // TODO: Some kind of checking for unused batches to
  //       save on memory after spikes
  for (const auto &vbo: primitive_vbos_)
    vbo->clear();
  curr_primitive_vbo_idx_ = 0;
}
