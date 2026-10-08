#include "baphy/glh/texture.hpp"

#include <utility>
#include "baphy/log.hpp"
#include "stb_image.h"

baphy::glh::Texture::Texture(const std::filesystem::path &filename, TextureScaling scaling) {
    glCreateTextures(GL_TEXTURE_2D, 1, &id);

    switch (scaling) {
    case TextureScaling::linear:
        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    case TextureScaling::retro:
        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    }

    const auto bytes = stbi_load(filename.c_str(), &w, &h, nullptr, STBI_rgb_alpha);
    if (!bytes)
        throw std::runtime_error("Failed to load texture: " + filename.string());

    glTextureStorage2D(id, 1, GL_RGBA8, w, h);
    glTextureSubImage2D(id, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    stbi_image_free(bytes);
}

baphy::glh::Texture::~Texture() {
    if (id)
        glDeleteTextures(1, &id);
}

baphy::glh::Texture::Texture(Texture &&other) noexcept
    : id(std::exchange(other.id, 0)),
      w(std::exchange(other.w, 0)),
      h(std::exchange(other.h, 0)) {}

baphy::glh::Texture &baphy::glh::Texture::operator=(Texture &&other) noexcept {
    if (this != &other) {
        if (id)
            glDeleteTextures(1, &id);

        id = std::exchange(other.id, 0);
        w = std::exchange(other.w, 0);
        h = std::exchange(other.h, 0);
    }
    return *this;
}

std::unique_ptr<baphy::glh::Texture>
baphy::glh::load_texture(const std::filesystem::path &filename, TextureScaling scaling) {
    try {
        return std::make_unique<baphy::glh::Texture>(filename, scaling);
    } catch (const std::exception &e) {
        BAPHY_LOG_ERROR("Failed to load texture {}: {}", filename.string(), e.what());
        return nullptr;
    }
}
