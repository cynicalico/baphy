#ifndef BAPHY_DETAIL_POLYLINE_HPP
#define BAPHY_DETAIL_POLYLINE_HPP

/**
 * I have searched for a long time for resources on drawing polylines and have
 * had basically zero luck. There aren't any libraries I can find that seem
 * viable for integrating into the library either.
 *
 * Out of desperation, since I do not know how to figure out the math myself,
 * I asked Claude to produce a polyline triangulation for me. Since I do not
 * entirely understand how this code works, I have decided to put it in its own
 * file to keep it somewhat isolated from everything else.
 */

#include <array>
#include <glm/glm.hpp>
#include <span>
#include <vector>

namespace baphy {
enum class LineJoin {
    bevel,
    miter,
    round,
};

namespace detail {
void triangulate_polyline(std::span<const glm::vec2> points,
                          float width,
                          bool closed,
                          LineJoin join,
                          std::vector<std::array<glm::vec2, 3>> &out);
} // namespace detail
} // namespace baphy

#endif // BAPHY_DETAIL_POLYLINE_HPP
