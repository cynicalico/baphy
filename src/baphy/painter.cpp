#include "baphy/painter.hpp"
#include <array>
#include <glm/gtc/type_ptr.hpp>

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

void baphy::Painter::tri(
    glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, const Color &color) {
  if (!primitive_vbos_[curr_primitive_vbo_idx_]->can_fit(3)) {
    if (primitive_vbos_.size() < curr_primitive_vbo_idx_) {
      primitive_vbos_.emplace_back(
          std::make_unique<glh::VecBuffer<PrimitiveVertex>>(
              PRIMITIVE_BATCH_SIZE));
    }
    curr_primitive_vbo_idx_++;
  }

  const auto gl_color = color.value();
  primitive_vbos_[curr_primitive_vbo_idx_]->extend(std::array{
      PrimitiveVertex{{p0.x, p0.y, 0.0f}, {gl_color.r, gl_color.g, gl_color.b}},
      PrimitiveVertex{{p1.x, p1.y, 0.0f}, {gl_color.r, gl_color.g, gl_color.b}},
      PrimitiveVertex{{p2.x, p2.y, 0.0f}, {gl_color.r, gl_color.g, gl_color.b}},
  });
}

void baphy::Painter::rect(glm::vec2 p, glm::vec2 size, const Color &color) {
  tri(p, p + glm::vec2{size.x, 0.0f}, p + size, color);
  tri(p, p + size, p + glm::vec2{0.0f, size.y}, color);
}

void baphy::Painter::draw(const glm::mat4 &projection) {
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

  // TODO: Some kind of checking for unused batches to
  //       save on memory after spikes
  for (const auto &vbo: primitive_vbos_)
    vbo->clear();
  curr_primitive_vbo_idx_ = 0;
}
