#include <array>
#include <filesystem>
#include <optional>
#include "baphy/baphy.hpp"

std::optional<GLuint>
create_shader_from_src(const char *vert_src, const char *frag_src);

std::optional<GLuint>
create_shader_from_file(const std::filesystem::path &vert_path,
                        const std::filesystem::path &frag_path);

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
    const auto cwd = std::filesystem::current_path();
    shader =
        create_shader_from_file(cwd / "examples" / "shaders" / "triangles.vert",
                                cwd / "examples" / "shaders" / "triangles.frag")
            .value();

    glCreateBuffers(1, &vbo);

    glNamedBufferStorage(
        vbo, sizeof(VERTEX_DATA), VERTEX_DATA.data(), GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &vao);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(
        vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribBinding(vao, 0, 0);

    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(
        vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
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

std::optional<GLuint>
create_shader_from_file(const std::filesystem::path &vert_path,
                        const std::filesystem::path &frag_path) {
  const auto vert_src = baphy::slurp(vert_path);
  const auto frag_src = baphy::slurp(frag_path);

  if (!vert_src || !frag_src)
    return std::nullopt;

  return create_shader_from_src(
      vert_src.value().c_str(), frag_src.value().c_str());
}
