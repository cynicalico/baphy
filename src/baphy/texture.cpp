#include "baphy/texture.hpp"

baphy::Texture::Texture(const std::filesystem::path &filename, const glh::TextureScaling scaling)
    : gl_texture_(filename, scaling),
      px_(1.0f / static_cast<float>(gl_texture_.w())),
      py_(1.0f / static_cast<float>(gl_texture_.h())) {}

GLuint baphy::Texture::id() const {
    return gl_texture_.id;
}

glm::ivec2 baphy::Texture::size() const {
    return {w(), h()};
}

int baphy::Texture::w() const {
    return gl_texture_.w();
}

int baphy::Texture::h() const {
    return gl_texture_.h();
}

glm::vec2 baphy::Texture::size_f() const {
    return {static_cast<float>(w()), static_cast<float>(h())};
}

float baphy::Texture::w_f() const {
    return static_cast<float>(w());
}

float baphy::Texture::h_f() const {
    return static_cast<float>(h());
}

glm::vec2 baphy::Texture::to_tex_coords(glm::ivec2 pos) const {
    return {static_cast<float>(pos.x) * px_, static_cast<float>(pos.y) * py_};
}
