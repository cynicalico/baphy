#include "baphy/detail/shader_uniform_loc_cache.hpp"

#include "baphy/log.hpp"

GLint baphy::detail::ShaderUniformLocCache::find(const GLuint shader_id, const std::string &name) {
    auto it = locs_[shader_id].find(name);
    if (it == locs_[shader_id].end()) {
        const auto loc = glGetUniformLocation(shader_id, name.c_str());
        if (loc == -1)
            BAPHY_LOG_WARN("Failed to find uniform: '{}'", name);
        it = locs_[shader_id].emplace_hint(it, name, loc);
    }
    return it->second;
}
