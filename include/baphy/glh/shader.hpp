#ifndef BAPHY_GLH_SHADER_HPP
#define BAPHY_GLH_SHADER_HPP

#include <filesystem>
#include <optional>
#include "baphy/gl.hpp"

namespace baphy::glh {
std::optional<GLuint>
create_shader_from_src(const char *vert_src, const char *frag_src);

std::optional<GLuint>
create_shader_from_file(const std::filesystem::path &vert_path,
                        const std::filesystem::path &frag_path);
} // namespace baphy::glh

#endif // BAPHY_GLH_SHADER_HPP
