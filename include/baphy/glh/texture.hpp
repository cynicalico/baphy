#ifndef BAPHY_GLH_TEXTURE_HPP
#define BAPHY_GLH_TEXTURE_HPP

#include <filesystem>
#include <memory>
#include <optional>
#include "baphy/gl.hpp"

namespace baphy::glh {
enum class TextureScaling {
    linear,
    retro
};

class Texture {
public:
    GLuint id{0};
    int w{0};
    int h{0};

    Texture(const std::filesystem::path &filename, TextureScaling scaling);

    ~Texture();

    Texture(const Texture &other) = delete;
    Texture(Texture &&other) noexcept;

    Texture &operator=(const Texture &other) = delete;
    Texture &operator=(Texture &&other) noexcept;
};

std::unique_ptr<Texture> load_texture(const std::filesystem::path &filename,
                                      TextureScaling scaling = TextureScaling::retro);
} // namespace baphy::glh

#endif // BAPHY_GLH_TEXTURE_HPP
