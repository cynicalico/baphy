#include "baphy/util/math.hpp"

glm::vec2 baphy::perp(const glm::vec2 d) {
  return {-d.y, d.x};
}

glm::vec2
baphy::point_on_ellipse(glm::vec2 center, glm::vec2 size, float theta) {
  return {center.x + size.x * std::cos(theta),
          center.y + size.y * std::sin(theta)};
}
