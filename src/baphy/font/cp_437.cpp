#include "baphy/font/cp_437.hpp"

#include "baphy/util/io.hpp"

baphy::CP437_Font::CP437_Font(Painter &painter, const std::filesystem::path &path, glm::vec2 glyph_size)
    : painter_(painter),
      texture_(std::make_unique<Texture>(path)),
      glyph_size_(glyph_size) {}

glm::vec2 baphy::CP437_Font::calc_bounds(std::string_view str, float scale) const {
    glm::vec2 bounds = {0, 0};

    float curr_x = 0.0f;
    for (const char &c: str | views::normalize_lf) {
        if (c == '\n') {
            bounds.y += glyph_size_.y;
            bounds.x = std::max(bounds.x, curr_x);
            curr_x = 0;
        } else {
            curr_x += glyph_size_.x;
        }
    }
    bounds.x = std::max(bounds.x, curr_x);

    if (bounds.y == 0 && bounds.x > 0)
        bounds.y += glyph_size_.y;

    return bounds * scale;
}

void baphy::CP437_Font::draw(std::string_view str, const glm::vec2 pos, float scale, const Color &color) const {
    glm::vec2 curr_pos = pos;
    for (const char &c: str | views::normalize_lf) {
        if (c == '\n') {
            curr_pos.x = pos.x;
            curr_pos.y += glyph_size_.y * scale;
        } else {
            const auto glyph_tex_pos = glm::vec2(
                    glyph_size_.x * static_cast<float>(c % 16), glyph_size_.y * static_cast<float>(c / 16 - 2));
            painter_.draw_tex_region(
                    *texture_, curr_pos, glm::vec2(glyph_size_) * scale, glyph_tex_pos, glm::vec2(glyph_size_), color);
            curr_pos.x += glyph_size_.x * scale;
        }
    }
}
