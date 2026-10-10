#include "baphy/detail/ellipse.hpp"

#include <stack>
#include <tuple>
#include "baphy/util/math.hpp"

void baphy::detail::triangulate_arc(
        const glm::vec2 center,
        const glm::vec2 size,
        const glm::vec2 a,
        const float a_t,
        const glm::vec2 b,
        const float b_t,
        const EllipseTriFn &f) {
    auto arcs = std::stack<std::tuple<glm::vec2, float, glm::vec2, float>>();
    arcs.emplace(a, a_t, b, b_t);

    while (!arcs.empty()) {
        const auto [pa, ta, pb, tb] = arcs.top();
        arcs.pop();

        const auto tm = (ta + tb) / 2.0f;
        const auto pm = point_on_ellipse(center, size, tm);

        const auto mid = (pa + pb) / 2.0f;
        const auto dist2 = glm::dot(pm - mid, pm - mid);
        // skip any vectors with a length less than 0.5
        if (dist2 >= 0.5 * 0.5) {
            f(pa, ta, pm, tm, pb, tb);

            arcs.emplace(pa, ta, pm, tm);
            arcs.emplace(pm, tm, pb, tb);
        }
    }
}

void baphy::detail::triangulate_ellipse(const glm::vec2 center, const glm::vec2 size, const EllipseTriFn &f) {
    constexpr auto t0 = glm::radians(0.0f);
    const auto p0 = point_on_ellipse(center, size, t0);

    constexpr auto t1 = glm::radians(120.0f);
    const auto p1 = point_on_ellipse(center, size, t1);

    constexpr auto t2 = glm::radians(240.0f);
    const auto p2 = point_on_ellipse(center, size, t2);

    f(p0, t0, p1, t1, p2, t2);

    // we need this because the midpoint calculation won't work otherwise
    constexpr auto t3 = glm::radians(360.0f);
    const auto p3 = point_on_ellipse(center, size, t3);

    triangulate_arc(center, size, p0, t0, p1, t1, f);
    triangulate_arc(center, size, p1, t1, p2, t2, f);
    triangulate_arc(center, size, p2, t2, p3, t3, f);
}
