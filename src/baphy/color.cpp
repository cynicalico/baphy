#include "baphy/color.hpp"

baphy::Color::Color(const glm::vec4 color)
    : color(color) {}

glm::vec4 baphy::Color::value() const {
  return color;
}

baphy::RGBA::RGBA(const std::uint8_t r,
                  const std::uint8_t g,
                  const std::uint8_t b,
                  const std::uint8_t a)
    : r(r),
      g(g),
      b(b),
      a(a) {}

baphy::RGBA::operator Color() const {
  return Color({static_cast<double>(r) / 255.0,
                static_cast<double>(g) / 255.0,
                static_cast<double>(b) / 255.0,
                static_cast<double>(a) / 255.0});
}

baphy::RGBA
baphy::rgb(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b) {
  return {r, g, b, 255};
}

baphy::RGBA baphy::rgba(const std::uint8_t r,
                        const std::uint8_t g,
                        const std::uint8_t b,
                        const std::uint8_t a) {
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
