#include <array>
#include <optional>
#include "baphy/baphy.hpp"

constexpr auto VERT_SHADER_SRC = R"glsl(
#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 frag_color;

void main() {
  frag_color = aColor;

  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)glsl";

constexpr auto FRAG_SHADER_SRC = R"glsl(
#version 460 core

out vec4 FragColor;

in vec3 frag_color;

void main() {
  FragColor = vec4(frag_color, 1.0f);
}
)glsl";

std::optional<GLuint>
create_shader_from_src(const char *vert_src, const char *frag_src);

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
};

constexpr std::array VERTEX_DATA = {
    Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
    Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
    Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
};

class Example final : public baphy::Application {
public:
  baphy::Window &window;

  GLuint shader{0};
  GLuint vbo{0};
  GLuint vao{0};

  explicit Example(baphy::Runner &runner)
      : Application(runner),
        window(*runner.window) {
    shader = create_shader_from_src(VERT_SHADER_SRC, FRAG_SHADER_SRC).value();

    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(
        vbo, sizeof(VERTEX_DATA), VERTEX_DATA.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &vao);
    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(
        vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(
        vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
  }

  baphy::Color clear_color() override { return baphy::rgb(128, 64, 32); }

  void update(double dt) override {}

  void draw() override {
    glUseProgram(shader);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  void keyboard_callback(const baphy::KeyboardEvent &e) override {
    if (e.key == baphy::Key::Escape && e.action == baphy::Action::Up)
      window.set_should_close(true);
  }
};

int main(int, char *[]) {
  return baphy::run<Example>(
      {.title = "Example", .size = {1280, 960}, .resizable = true});
}

std::optional<GLuint>
create_shader_from_src(const char *vert_src, const char *frag_src) {
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
