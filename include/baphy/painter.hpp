#ifndef BAPHY_PAINTER_HPP
#define BAPHY_PAINTER_HPP

#include <glm/glm.hpp>
#include <memory>
#include <span>
#include <vector>
#include "baphy/color.hpp"
#include "baphy/detail/polyline.hpp"
#include "baphy/glh/glh.hpp"

namespace baphy {
struct PrimitiveVertex {
  glm::vec3 pos;
  glm::vec3 color;
};

class Painter {
public:
  Painter();
  ~Painter();

  Painter(const Painter &other) = delete;
  Painter(Painter &&other) noexcept = delete;

  Painter &operator=(const Painter &other) = delete;
  Painter &operator=(Painter &&other) noexcept = delete;

  void point(glm::vec2 p0, const Color &color);

  void line(glm::vec2 p0, glm::vec2 p1, float width, const Color &color);
  void polyline(std::span<const glm::vec2> points,
                float width,
                bool closed,
                LineJoin join,
                const Color &color);

  void tri(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, const Color &color);

  void rect(glm::vec2 p0, glm::vec2 size, const Color &color);
  void square(glm::vec2 p0, float size, const Color &color);

  void ellipse(glm::vec2 center, glm::vec2 size, const Color &color);
  void circle(glm::vec2 center, float size, const Color &color);

  void draw(const glm::mat4 &projection);

private:
  GLuint primitive_shader_{0};
  GLuint primitive_vao_{0};
  std::vector<std::unique_ptr<glh::VecBuffer<PrimitiveVertex>>>
      primitive_vbos_{};
  std::size_t curr_primitive_vbo_idx_{0};
};
} // namespace baphy

#endif // BAPHY_PAINTER_HPP
