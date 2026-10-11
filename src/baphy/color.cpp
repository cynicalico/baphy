#include "baphy/color.hpp"

#include <algorithm>
#include <cmath>

namespace {
struct HSL {
    float h;
    float s;
    float l;
};

HSL rgb_to_hsl(const float r, const float g, const float b) {
    const float max = std::max({r, g, b});
    const float min = std::min({r, g, b});
    const float l = (max + min) / 2.0f;

    if (max == min)
        return {0.0f, 0.0f, l};

    const float d = max - min;
    const float s = l > 0.5f ? d / (2.0f - max - min) : d / (max + min);

    float h;
    if (max == r)
        h = (g - b) / d + (g < b ? 6.0f : 0.0f);
    else if (max == g)
        h = (b - r) / d + 2.0f;
    else
        h = (r - g) / d + 4.0f;

    return {h / 6.0f, s, l};
}

float hue_to_rgb(const float p, const float q, float t) {
    if (t < 0.0f)
        t += 1.0f;
    if (t > 1.0f)
        t -= 1.0f;
    if (t < 1.0f / 6.0f)
        return p + (q - p) * 6.0f * t;
    if (t < 1.0f / 2.0f)
        return q;
    if (t < 2.0f / 3.0f)
        return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
    return p;
}

baphy::RGBA hsl_to_rgba(const HSL hsl, const std::uint8_t a) {
    const auto to_u8 = [](const float v) {
        return static_cast<std::uint8_t>(std::round(std::clamp(v, 0.0f, 1.0f) * 255.0f));
    };

    if (hsl.s == 0.0f)
        return {to_u8(hsl.l), to_u8(hsl.l), to_u8(hsl.l), a};

    const float q = hsl.l < 0.5f ? hsl.l * (1.0f + hsl.s) : hsl.l + hsl.s - hsl.l * hsl.s;
    const float p = 2.0f * hsl.l - q;
    return {to_u8(hue_to_rgb(p, q, hsl.h + 1.0f / 3.0f)),
            to_u8(hue_to_rgb(p, q, hsl.h)),
            to_u8(hue_to_rgb(p, q, hsl.h - 1.0f / 3.0f)),
            a};
}

HSL rgba_to_hsl(const baphy::RGBA &c) {
    return rgb_to_hsl(static_cast<float>(c.r) / 255.0f, static_cast<float>(c.g) / 255.0f, static_cast<float>(c.b) / 255.0f);
}
} // namespace

baphy::Color::Color(const glm::vec4 color)
    : color(color) {}

glm::vec4 baphy::Color::value() const {
    return color;
}

baphy::RGBA::RGBA(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a)
    : r(r),
      g(g),
      b(b),
      a(a) {}

baphy::RGBA baphy::RGBA::lighten(const float amount) const {
    HSL hsl = rgba_to_hsl(*this);
    hsl.l += (1.0f - hsl.l) * std::clamp(amount, 0.0f, 1.0f);
    return hsl_to_rgba(hsl, a);
}

baphy::RGBA baphy::RGBA::darken(const float amount) const {
    HSL hsl = rgba_to_hsl(*this);
    hsl.l *= 1.0f - std::clamp(amount, 0.0f, 1.0f);
    return hsl_to_rgba(hsl, a);
}

baphy::RGBA::operator Color() const {
    return Color({static_cast<double>(r) / 255.0,
                  static_cast<double>(g) / 255.0,
                  static_cast<double>(b) / 255.0,
                  static_cast<double>(a) / 255.0});
}

baphy::RGBA baphy::rgb(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) {
    return {r, g, b, 255};
}

baphy::RGBA baphy::rgba(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a) {
    return {r, g, b, a};
}

baphy::RGBA baphy::rgb(const std::uint32_t hex) {
    return {static_cast<std::uint8_t>((hex >> 16) & 0xff),
            static_cast<std::uint8_t>((hex >> 8) & 0xff),
            static_cast<std::uint8_t>(hex & 0xff),
            255};
}

baphy::RGBA baphy::rgba(const std::uint32_t hex) {
    return {static_cast<std::uint8_t>((hex >> 24) & 0xff),
            static_cast<std::uint8_t>((hex >> 16) & 0xff),
            static_cast<std::uint8_t>((hex >> 8) & 0xff),
            static_cast<std::uint8_t>(hex & 0xff)};
}
