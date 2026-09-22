#include "baphy/glh/shader.hpp"
#include "baphy/util/io.hpp"

std::optional<GLuint>
baphy::glh::create_shader_from_src(const char *vert_src, const char *frag_src) {
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

std::optional<GLuint>
baphy::glh::create_shader_from_file(const std::filesystem::path &vert_path,
                                    const std::filesystem::path &frag_path) {
  const auto vert_src = slurp(vert_path);
  const auto frag_src = slurp(frag_path);

  if (!vert_src || !frag_src)
    return std::nullopt;

  return create_shader_from_src(
      vert_src.value().c_str(), frag_src.value().c_str());
}
