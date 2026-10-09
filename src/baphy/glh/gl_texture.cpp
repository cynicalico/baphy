#include "baphy/glh/gl_texture.hpp"

#include <fmt/format.h>
#include <utility>
#include "baphy/log.hpp"
#include "stb_image.h"

baphy::glh::GlTexture::GlTexture(const std::filesystem::path &filename, TextureScaling scaling) {
    const auto bytes = stbi_load(filename.c_str(), &w_, &h_, nullptr, STBI_rgb_alpha);
    if (!bytes)
        throw std::runtime_error(fmt::format("Failed to load texture: {}", filename.string()));

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

    glTextureStorage2D(id, 1, GL_RGBA8, w_, h_);
    glTextureSubImage2D(id, 0, 0, 0, w_, h_, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    stbi_image_free(bytes);
}

baphy::glh::GlTexture::~GlTexture() {
    if (id)
        glDeleteTextures(1, &id);
}

baphy::glh::GlTexture::GlTexture(GlTexture &&other) noexcept
    : id(std::exchange(other.id, 0)),
      w_(std::exchange(other.w_, 0)),
      h_(std::exchange(other.h_, 0)) {}

baphy::glh::GlTexture &baphy::glh::GlTexture::operator=(GlTexture &&other) noexcept {
    if (this != &other) {
        if (id)
            glDeleteTextures(1, &id);

        id = std::exchange(other.id, 0);
        w_ = std::exchange(other.w_, 0);
        h_ = std::exchange(other.h_, 0);
    }
    return *this;
}

int baphy::glh::GlTexture::w() const {
    return w_;
}

int baphy::glh::GlTexture::h() const {
    return h_;
}
