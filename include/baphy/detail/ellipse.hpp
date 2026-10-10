#ifndef BAPHY_DETAIL_ELLIPSE_HPP
#define BAPHY_DETAIL_ELLIPSE_HPP

#include <functional>
#include <glm/glm.hpp>

namespace baphy::detail {
using EllipseTriFn = std::function<void(glm::vec2, float, glm::vec2, float, glm::vec2, float)>;

void triangulate_arc(
        glm::vec2 center, glm::vec2 size, glm::vec2 a, float a_t, glm::vec2 b, float b_t, const EllipseTriFn &f);

void triangulate_ellipse(glm::vec2 center, glm::vec2 size, const EllipseTriFn &f);
} // namespace baphy::detail

#endif // BAPHY_DETAIL_ELLIPSE_HPP
