#include "baphy/detail/polyline.hpp"

#include <algorithm>
#include <cmath>
#include "baphy/detail/ellipse.hpp"
#include "baphy/util/math.hpp"

constexpr auto EPSILON = 1e-6;

// max ratio of miter length to line width before falling back to a bevel,
// the same default as SVG and canvas (anything sharper than ~29 degrees)
constexpr auto MITER_LIMIT = 4.0f;

// miter length / width = 1 / cos(theta / 2), and cos^2(theta / 2) is
// (1 + along) / 2, so the limit can be checked against t_den without a sqrt
constexpr auto MITER_MIN_T_DEN = 2.0f / (MITER_LIMIT * MITER_LIMIT);

namespace {
struct Joint {
    glm::vec2 in_r, in_l; // r is the +perp side, right with y down
    glm::vec2 out_r, out_l;
};
} // namespace

static std::vector<glm::vec2> drop_duplicate_points(std::span<const glm::vec2> points, bool closed) {
    auto ps = std::vector<glm::vec2>();
    ps.reserve(points.size());
    for (const auto &p: points)
        if (ps.empty() || glm::dot(p - ps.back(), p - ps.back()) > EPSILON * EPSILON)
            ps.push_back(p);

    if (closed && ps.size() > 1 && glm::dot(ps.back() - ps.front(), ps.back() - ps.front()) <= EPSILON * EPSILON)
        ps.pop_back();

    return ps;
}

static float cross2d(const glm::vec2 v0, const glm::vec2 v1) {
    return v0.x * v1.y - v0.y * v1.x;
}

void baphy::detail::triangulate_polyline(std::span<const glm::vec2> points,
                                         const float width,
                                         bool closed,
                                         const LineJoin join,
                                         std::vector<std::array<glm::vec2, 3>> &out) {
    auto ps = drop_duplicate_points(points, closed);

    const auto n = ps.size();
    if (n < 2)
        return;

    if (n < 3)
        closed = false;

    const auto half_w = width / 2.0f;
    auto joints = std::vector<Joint>(n);

    const auto cap = [&](const glm::vec2 p, const glm::vec2 d) {
        const auto o = perp(d) * half_w;
        return Joint{p + o, p - o, p + o, p - o};
    };

    // fills between the chord a-b and the arc of radius half_w around p that
    // passes through both, subdivided the same way as ellipse
    const auto round_join = [&](const glm::vec2 p, const glm::vec2 a, const glm::vec2 b, const float sweep) {
        const auto ta = std::atan2(a.y - p.y, a.x - p.x);
        triangulate_arc(p,
                        {half_w, half_w},
                        a,
                        ta,
                        b,
                        ta + sweep,
                        [&](glm::vec2 p0, float, glm::vec2 p1, float, glm::vec2 p2, float) {
                            out.emplace_back(std::array{p0, p1, p2});
                        });
    };

    if (!closed) {
        joints.front() = cap(ps[0], glm::normalize(ps[1] - ps[0]));
        joints.back() = cap(ps[n - 1], glm::normalize(ps[n - 1] - ps[n - 2]));
    }

    const auto first = closed ? std::size_t{0} : std::size_t{1};
    const auto last = closed ? n : n - 1;
    for (std::size_t i = first; i < last; ++i) {
        const auto in_i = (i + n - 1) % n;
        const auto out_i = (i + 1) % n;

        const auto p = ps[i];

        const auto in_seg = p - ps[in_i];
        const auto out_seg = ps[out_i] - p;

        const auto in_seg_len = glm::length(in_seg);
        const auto out_seg_len = glm::length(out_seg);

        const auto in_seg_norm = in_seg / in_seg_len;
        const auto out_seg_norm = out_seg / out_seg_len;

        const auto n0 = perp(in_seg_norm) * half_w;
        const auto n1 = perp(out_seg_norm) * half_w;

        // > 0 means right, < 0 means left, 0 means straight
        const auto turn = cross2d(in_seg_norm, out_seg_norm);
        // > 0 means same direction, < 0 means different, 0 means perp
        const auto along = glm::dot(in_seg_norm, out_seg_norm);

        if (std::abs(turn) < EPSILON && along > 0.0f) {
            joints[i] = {p + n0, p - n0, p + n0, p - n0};
            continue;
        }

        // how far the inner corner slides back along each segment
        const auto t_num = half_w * std::abs(turn);
        const auto t_den = 1.0f + along;

        // miters get longer as the corner gets sharper, past the limit use a bevel
        const auto use_miter = join == LineJoin::miter && t_den >= MITER_MIN_T_DEN;

        // offsets from p to the outer corners of the incoming and outgoing segments
        const auto outer_n0 = turn > 0.0f ? -n0 : n0;
        const auto outer_n1 = turn > 0.0f ? -n1 : n1;

        // t < min(l0, l1) / 2, avoiding dividing by t_den when t_den is very small
        // (strict so a 180 degree reversal, where both are 0, goes to the fallback)
        if (2.0f * t_num < std::min(in_seg_len, out_seg_len) * t_den) {
            const auto t = t_num / t_den;

            if (turn > 0.0f) {
                const auto inner = p + n0 - in_seg_norm * t;
                if (use_miter) {
                    const auto outer = p - n0 + in_seg_norm * t;
                    joints[i] = {inner, outer, inner, outer};
                } else {
                    joints[i] = {inner, p - n0, inner, p - n1};
                    out.emplace_back(std::array{inner, p - n0, p - n1});
                }
            } else {
                const auto inner = p - n0 - in_seg_norm * t;
                if (use_miter) {
                    const auto outer = p + n0 + in_seg_norm * t;
                    joints[i] = {outer, inner, outer, inner};
                } else {
                    joints[i] = {p + n0, inner, p + n1, inner};
                    out.emplace_back(std::array{inner, p + n0, p + n1});
                }
            }
        } else {
            // too sharp, overlap ends instead
            joints[i] = {p + n0, p - n0, p + n1, p - n1};

            // the outer side is the same as above, just filled from p
            if (use_miter) {
                const auto t = t_num / t_den;
                const auto outer = p + outer_n0 + in_seg_norm * t;
                out.emplace_back(std::array{p, p + outer_n0, outer});
                out.emplace_back(std::array{p, outer, p + outer_n1});
            } else {
                out.emplace_back(std::array{p, p + outer_n0, p + outer_n1});
            }
        }

        // a round join is a bevel with the outside of the chord filled out to the
        // arc, the sweep is signed so it goes around the outside even for a
        // 180 degree reversal, where both outer corners are directly opposite
        if (join == LineJoin::round) {
            const auto sweep = (turn > 0.0f ? 1.0f : -1.0f) * std::atan2(std::abs(turn), along);
            round_join(p, p + outer_n0, p + outer_n1, sweep);
        }
    }

    const auto segments = closed ? n : n - 1;
    for (std::size_t i = 0; i < segments; ++i) {
        const auto &s = joints[i];
        const auto &e = joints[(i + 1) % n];

        out.emplace_back(std::array{s.out_r, e.in_r, e.in_l});
        out.emplace_back(std::array{s.out_r, e.in_l, s.out_l});
    }
}
