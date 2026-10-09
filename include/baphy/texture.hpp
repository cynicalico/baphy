#ifndef BAPHY_TEXTURE_HPP
#define BAPHY_TEXTURE_HPP

#include <glm/glm.hpp>
#include "baphy/glh/gl_texture.hpp"

namespace baphy {
class Texture {
public:
    Texture(const std::filesystem::path &filename, glh::TextureScaling scaling = glh::TextureScaling::retro);

    [[nodiscard]] GLuint id() const;

    [[nodiscard]] glm::ivec2 size() const;
    [[nodiscard]] int w() const;
    [[nodiscard]] int h() const;

    [[nodiscard]] glm::vec2 size_f() const;
    [[nodiscard]] float w_f() const;
    [[nodiscard]] float h_f() const;

    [[nodiscard]] glm::vec2 to_tex_coords(glm::ivec2 pos) const;

private:
    glh::GlTexture gl_texture_;
    float px_;
    float py_;
};
} // namespace baphy

#endif // BAPHY_TEXTURE_HPP
