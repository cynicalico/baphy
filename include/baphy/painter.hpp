#ifndef BAPHY_PAINTER_HPP
#define BAPHY_PAINTER_HPP

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "baphy/color.hpp"
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

  void tri(glm::vec2 p0, glm::vec2 p1, glm::vec2 p2, const Color &color);
  void rect(glm::vec2 p0, glm::vec2 size, const Color &color);

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
