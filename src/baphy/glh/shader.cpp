#include "baphy/glh/shader.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <optional>
#include <utility>
#include "baphy/log.hpp"
#include "baphy/util/io.hpp"

static std::optional<GLuint> create_shader_from_src(const char *vert_src, const char *frag_src);

std::unique_ptr<baphy::glh::Shader> baphy::glh::Shader::from_src(const char *vert_src, const char *frag_src) {
    return std::unique_ptr<Shader>(new Shader(vert_src, frag_src));
}

std::unique_ptr<baphy::glh::Shader>
baphy::glh::Shader::from_file(const std::filesystem::path &vert_path, const std::filesystem::path &frag_path) {
    const auto vert_src = slurp(vert_path);
    const auto frag_src = slurp(frag_path);

    if (!vert_src || !frag_src)
        throw std::runtime_error("Failed to read shader source");

    return from_src(vert_src.value().c_str(), frag_src.value().c_str());
}

baphy::glh::Shader::~Shader() {
    if (id != 0)
        glDeleteProgram(id);
}

baphy::glh::Shader::Shader(Shader &&other) noexcept
    : id(std::exchange(other.id, 0)) {}

baphy::glh::Shader &baphy::glh::Shader::operator=(Shader &&other) noexcept {
    if (this != &other) {
        if (id != 0)
            glDeleteProgram(id);

        id = std::exchange(other.id, 0);
    }
    return *this;
}

void baphy::glh::Shader::uniform_1f(std::string_view name, GLfloat v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform1f(id, loc, v0);
}

void baphy::glh::Shader::uniform_2f(std::string_view name, glm::vec2 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform2fv(id, loc, 1, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_3f(std::string_view name, glm::vec3 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform3fv(id, loc, 1, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_4f(std::string_view name, glm::vec4 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform4fv(id, loc, 1, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_1i(std::string_view name, GLint v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform1i(id, loc, v0);
}

void baphy::glh::Shader::uniform_2i(std::string_view name, glm::ivec2 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform2iv(id, loc, 1, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_3i(std::string_view name, glm::ivec3 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform3iv(id, loc, 1, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_4i(std::string_view name, glm::ivec4 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform4iv(id, loc, 1, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_1ui(std::string_view name, GLuint v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform1ui(id, loc, v0);
}

void baphy::glh::Shader::uniform_2ui(std::string_view name, glm::uvec2 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform2uiv(id, loc, 1, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_3ui(std::string_view name, glm::uvec3 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform3uiv(id, loc, 1, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_4ui(std::string_view name, glm::uvec4 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform4uiv(id, loc, 1, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_1fv(std::string_view name, GLsizei count, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform1fv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_2fv(std::string_view name, GLsizei count, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform2fv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_3fv(std::string_view name, GLsizei count, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform3fv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_4fv(std::string_view name, GLsizei count, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform4fv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_1iv(std::string_view name, GLsizei count, const GLint *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform1iv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_2iv(std::string_view name, GLsizei count, const GLint *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform2iv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_3iv(std::string_view name, GLsizei count, const GLint *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform3iv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_4iv(std::string_view name, GLsizei count, const GLint *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform4iv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_1uiv(std::string_view name, GLsizei count, const GLuint *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform1uiv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_2uiv(std::string_view name, GLsizei count, const GLuint *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform2uiv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_3uiv(std::string_view name, GLsizei count, const GLuint *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform3uiv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_4uiv(std::string_view name, GLsizei count, const GLuint *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniform4uiv(id, loc, count, value);
}

void baphy::glh::Shader::uniform_mat2f(std::string_view name, bool transpose, glm::mat2 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix2fv(id, loc, 1, transpose, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_mat3f(std::string_view name, bool transpose, glm::mat3 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix3fv(id, loc, 1, transpose, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_mat4f(std::string_view name, bool transpose, glm::mat4 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix4fv(id, loc, 1, transpose, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_mat2x3f(std::string_view name, bool transpose, glm::mat2x3 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix2x3fv(id, loc, 1, transpose, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_mat3x2f(std::string_view name, bool transpose, glm::mat3x2 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix3x2fv(id, loc, 1, transpose, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_mat2x4f(std::string_view name, bool transpose, glm::mat2x4 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix2x4fv(id, loc, 1, transpose, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_mat4x2f(std::string_view name, bool transpose, glm::mat4x2 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix4x2fv(id, loc, 1, transpose, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_mat3x4f(std::string_view name, bool transpose, glm::mat3x4 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix3x4fv(id, loc, 1, transpose, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_mat4x3f(std::string_view name, bool transpose, glm::mat4x3 v0) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix4x3fv(id, loc, 1, transpose, glm::value_ptr(v0));
}

void baphy::glh::Shader::uniform_mat2fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix2fv(id, loc, count, transpose, value);
}

void baphy::glh::Shader::uniform_mat3fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix3fv(id, loc, count, transpose, value);
}

void baphy::glh::Shader::uniform_mat4fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix4fv(id, loc, count, transpose, value);
}

void baphy::glh::Shader::uniform_mat2x3fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix2x3fv(id, loc, count, transpose, value);
}

void baphy::glh::Shader::uniform_mat3x2fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix3x2fv(id, loc, count, transpose, value);
}

void baphy::glh::Shader::uniform_mat2x4fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix2x4fv(id, loc, count, transpose, value);
}

void baphy::glh::Shader::uniform_mat4x2fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix4x2fv(id, loc, count, transpose, value);
}

void baphy::glh::Shader::uniform_mat3x4fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix3x4fv(id, loc, count, transpose, value);
}

void baphy::glh::Shader::uniform_mat4x3fv(std::string_view name, GLsizei count, bool transpose, const GLfloat *value) {
    if (const auto loc = find_uniform_loc_(name); loc != -1)
        glProgramUniformMatrix4x3fv(id, loc, count, transpose, value);
}

baphy::glh::Shader::Shader(const char *vert_src, const char *frag_src) {
    const auto id_opt = create_shader_from_src(vert_src, frag_src);
    if (!id_opt.has_value())
        throw std::runtime_error("Failed to create shader");
    id = id_opt.value();
}

GLint baphy::glh::Shader::find_uniform_loc_(std::string_view name) {
    auto it = uniform_loc_cache_.find(std::string(name));
    if (it == uniform_loc_cache_.end()) {
        const auto loc = glGetUniformLocation(id, name.data());
        if (loc == -1)
            BAPHY_LOG_WARN("Failed to find uniform: '{}'", name);
        it = uniform_loc_cache_.emplace_hint(it, name, loc);
    }
    return it->second;
}

static std::optional<GLuint> create_shader_from_src(const char *vert_src, const char *frag_src) {
    const GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vert_src, nullptr);
    glCompileShader(vert_shader);
    GLint vert_compiled;
    glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &vert_compiled);
    if (vert_compiled == GL_FALSE) {
        glDeleteShader(vert_shader);

        return std::nullopt;
    }

    const GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_src, nullptr);
    glCompileShader(frag_shader);
    GLint frag_compiled;
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &frag_compiled);
    if (frag_compiled == GL_FALSE) {
        glDeleteShader(vert_shader);
        glDeleteShader(frag_shader);

        return std::nullopt;
    }

    const auto shader = glCreateProgram();
    glAttachShader(shader, vert_shader);
    glAttachShader(shader, frag_shader);
    glLinkProgram(shader);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    GLint link_status;
    glGetProgramiv(shader, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        glDeleteProgram(shader);

        return std::nullopt;
    }

    return shader;
}
