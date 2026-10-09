#ifndef BAPHY_GLH_TEXTURE_HPP
#define BAPHY_GLH_TEXTURE_HPP

#include <filesystem>
#include <memory>
#include "baphy/gl.hpp"

namespace baphy::glh {
enum class TextureScaling {
    linear,
    retro
};

class GlTexture {
public:
    GLuint id{0};

    GlTexture(const std::filesystem::path &filename, TextureScaling scaling);

    ~GlTexture();

    GlTexture(const GlTexture &other) = delete;
    GlTexture(GlTexture &&other) noexcept;

    GlTexture &operator=(const GlTexture &other) = delete;
    GlTexture &operator=(GlTexture &&other) noexcept;

    [[nodiscard]] int w() const;
    [[nodiscard]] int h() const;

private:
    int w_{0};
    int h_{0};
};
} // namespace baphy::glh

#endif // BAPHY_GLH_TEXTURE_HPP
