#include "baphy/util/math.hpp"

glm::vec2 baphy::perp(const glm::vec2 d) {
    return {-d.y, d.x};
}

glm::vec2 baphy::point_on_ellipse(glm::vec2 center, glm::vec2 size, float theta) {
    return {center.x + size.x * std::cos(theta), center.y + size.y * std::sin(theta)};
}

std::vector<glm::ivec2> baphy::bresenham(glm::ivec2 start, glm::ivec2 end) {
    std::vector<glm::ivec2> points;

    const auto bresenham_low = [&points](const glm::ivec2 p0, const glm::ivec2 p1) {
        const int dx = p1.x - p0.x;
        int dy = p1.y - p0.y;
        int yi = 1;
        if (dy < 0) {
            yi = -1;
            dy = -dy;
        }
        int D = 2 * dy - dx;
        int y = p0.y;

        for (int x = p0.x; x <= p1.x; x++) {
            points.emplace_back(x, y);
            if (D > 0) {
                y += yi;
                D += 2 * (dy - dx);
            } else {
                D += 2 * dy;
            }
        }
    };

    const auto bresenham_high = [&points](const glm::ivec2 p0, const glm::ivec2 p1) {
        int dx = p1.x - p0.x;
        const int dy = p1.y - p0.y;
        int xi = 1;
        if (dx < 0) {
            xi = -1;
            dx = -dx;
        }
        int D = 2 * dx - dy;
        int x = p0.x;

        for (int y = p0.y; y <= p1.y; y++) {
            points.emplace_back(x, y);
            if (D > 0) {
                x += xi;
                D += 2 * (dx - dy);
            } else {
                D += 2 * dx;
            }
        }
    };

    if (std::abs(end.y - start.y) < std::abs(end.x - start.x)) {
        if (start.x > end.x)
            std::swap(start, end);
        bresenham_low(start, end);
    } else {
        if (start.y > end.y)
            std::swap(start, end);
        bresenham_high(start, end);
    }
    return points;
}
