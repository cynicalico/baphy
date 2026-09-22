#include <array>
#include "baphy/baphy.hpp"

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
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
    shader = baphy::glh::create_shader_from_file(
                 cwd / "examples" / "shaders" / "triangles.vert",
                 cwd / "examples" / "shaders" / "triangles.frag")
                 .value();

    vbo = baphy::glh::create_buffer_from_data(std::array{
        Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
        Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)},
        Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)},
    });

    vao = baphy::glh::create_vertex_array_from_bindings(
        shader,
        {{"aPos", 3, GL_FLOAT, false, offsetof(Vertex, pos)},
         {"aColor", 3, GL_FLOAT, false, offsetof(Vertex, color)}});

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
  }

  baphy::Color clear_color() override { return baphy::rgb(128, 64, 32); }

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
