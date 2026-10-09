#ifndef BAPHY_DETAIL_SHADER_UNIFORM_LOC_CACHE_HPP
#define BAPHY_DETAIL_SHADER_UNIFORM_LOC_CACHE_HPP

#include <string>
#include <unordered_map>
#include "baphy/gl.hpp"

namespace baphy::detail {
class ShaderUniformLocCache {
public:
    ShaderUniformLocCache() = default;

    GLint find(GLuint shader_id, const std::string &name);

private:
    std::unordered_map<GLuint, std::unordered_map<std::string, GLint>> locs_{};
};
} // namespace baphy::detail

#endif // BAPHY_DETAIL_SHADER_UNIFORM_LOC_CACHE_HPP
