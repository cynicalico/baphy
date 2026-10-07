#ifndef BAPHY_UTIL_MATH_HPP
#define BAPHY_UTIL_MATH_HPP

#include <glm/glm.hpp>

namespace baphy {
glm::vec2 perp(const glm::vec2 d);
glm::vec2 point_on_ellipse(glm::vec2 center, glm::vec2 size, float theta);
} // namespace baphy

#endif // BAPHY_UTIL_MATH_HPP
