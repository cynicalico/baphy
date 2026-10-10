#ifndef BAPHY_GLH_SHADER_HPP
#define BAPHY_GLH_SHADER_HPP

#include <filesystem>
#include <glm/glm.hpp>
#include <unordered_map>
#include "baphy/gl.hpp"

namespace baphy::glh {
class Shader {
public:
    GLuint id{0};

    static std::unique_ptr<Shader> from_src(const char *vert_src, const char *frag_src);

    static std::unique_ptr<Shader>
    from_file(const std::filesystem::path &vert_path, const std::filesystem::path &frag_path);

    ~Shader();

    Shader(const Shader &other) = delete;
    Shader &operator=(const Shader &other) = delete;

    Shader(Shader &&other) noexcept;
    Shader &operator=(Shader &&other) noexcept;

    void uniform_1f(std::string_view name, GLfloat v0);
    void uniform_2f(std::string_view name, glm::vec2 v0);
    void uniform_3f(std::string_view name, glm::vec3 v0);
    void uniform_4f(std::string_view name, glm::vec4 v0);

    void uniform_1i(std::string_view name, GLint v0);
    void uniform_2i(std::string_view name, glm::ivec2 v0);
    void uniform_3i(std::string_view name, glm::ivec3 v0);
    void uniform_4i(std::string_view name, glm::ivec4 v0);

    void uniform_1ui(std::string_view name, GLuint v0);
    void uniform_2ui(std::string_view name, glm::uvec2 v0);
    void uniform_3ui(std::string_view name, glm::uvec3 v0);
    void uniform_4ui(std::string_view name, glm::uvec4 v0);

    void uniform_1fv(std::string_view name, GLsizei count, const GLfloat *value);
    void uniform_2fv(std::string_view name, GLsizei count, const GLfloat *value);
    void uniform_3fv(std::string_view name, GLsizei count, const GLfloat *value);
    void uniform_4fv(std::string_view name, GLsizei count, const GLfloat *value);

    void uniform_1iv(std::string_view name, GLsizei count, const GLint *value);
    void uniform_2iv(std::string_view name, GLsizei count, const GLint *value);
    void uniform_3iv(std::string_view name, GLsizei count, const GLint *value);
    void uniform_4iv(std::string_view name, GLsizei count, const GLint *value);

    void uniform_1uiv(std::string_view name, GLsizei count, const GLuint *value);
    void uniform_2uiv(std::string_view name, GLsizei count, const GLuint *value);
    void uniform_3uiv(std::string_view name, GLsizei count, const GLuint *value);
    void uniform_4uiv(std::string_view name, GLsizei count, const GLuint *value);

    void uniform_mat2f(std::string_view name, bool transpose, glm::mat2 v0);
    void uniform_mat3f(std::string_view name, bool transpose, glm::mat3 v0);
    void uniform_mat4f(std::string_view name, bool transpose, glm::mat4 v0);
    void uniform_mat2x3f(std::string_view name, bool transpose, glm::mat2x3 v0);
    void uniform_mat3x2f(std::string_view name, bool transpose, glm::mat3x2 v0);
    void uniform_mat2x4f(std::string_view name, bool transpose, glm::mat2x4 v0);
    void uniform_mat4x2f(std::string_view name, bool transpose, glm::mat4x2 v0);
    void uniform_mat3x4f(std::string_view name, bool transpose, glm::mat3x4 v0);
    void uniform_mat4x3f(std::string_view name, bool transpose, glm::mat4x3 v0);

    void uniform_mat2fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value);
    void uniform_mat3fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value);
    void uniform_mat4fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value);
    void uniform_mat2x3fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value);
    void uniform_mat3x2fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value);
    void uniform_mat2x4fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value);
    void uniform_mat4x2fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value);
    void uniform_mat3x4fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value);
    void uniform_mat4x3fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value);

private:
    Shader(const char *vert_src, const char *frag_src);

    std::unordered_map<std::string, GLint> uniform_loc_cache_{};
    GLint find_uniform_loc_(std::string_view name);
};
} // namespace baphy::glh

#endif // BAPHY_GLH_SHADER_HPP
