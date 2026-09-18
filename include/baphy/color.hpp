#ifndef BAPHY_COLOR_HPP
#define BAPHY_COLOR_HPP

#include <glm/vec4.hpp>

namespace baphy {
class Color {
public:
  Color() = default;
  explicit Color(glm::vec4 color);

  [[nodiscard]] glm::vec4 value() const;

private:
  glm::vec4 color{0.0f, 0.0f, 0.0f, 1.0f};
};

class RGBA {
public:
  std::uint8_t r{0};
  std::uint8_t g{0};
  std::uint8_t b{0};
  std::uint8_t a{0};

  RGBA() = default;
  RGBA(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

  [[nodiscard]] operator Color() const;
};

RGBA rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);
RGBA rgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
RGBA rgb(std::uint32_t hex);
RGBA rgba(std::uint32_t hex);
} // namespace baphy

#endif // BAPHY_COLOR_HPP
