#ifndef BAPHY_FONT_CP_437_HPP
#define BAPHY_FONT_CP_437_HPP

#include <glm/glm.hpp>
#include "baphy/painter.hpp"
#include "baphy/texture.hpp"

namespace baphy {
class CP437_Font {
public:
    CP437_Font(Painter &painter, const std::filesystem::path &path, glm::vec2 glyph_size);

    [[nodiscard]] glm::vec2 glyph_size() const { return glyph_size_; }
    [[nodiscard]] const Texture &texture() const { return *texture_; }

    [[nodiscard]] glm::vec2 calc_bounds(std::string_view str, float scale = 1.0f) const;

    void draw(std::string_view str,
              glm::vec2 pos,
              float scale = 1.0f,
              const Color &color = rgb(255, 255, 255)) const;

private:
    Painter &painter_;

    std::unique_ptr<Texture> texture_;
    glm::vec2 glyph_size_;
};
} // namespace baphy

#endif // BAPHY_FONT_CP_437_HPP
