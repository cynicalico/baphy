#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "baphy/baphy.hpp"

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
};

class Example final : public baphy::Application {
public:
  baphy::Window &window;

  GLuint shader{0};

  std::unique_ptr<baphy::glh::VecBuffer<Vertex>> vbo;
  GLuint vao{0};

  explicit Example(baphy::Runner &runner)
      : Application(runner),
        window(*runner.window) {
    const auto cwd = std::filesystem::current_path();
    shader = baphy::glh::create_shader_from_file(
                 cwd / "examples" / "shaders" / "triangles.vert",
                 cwd / "examples" / "shaders" / "triangles.frag")
                 .value();

    vbo = std::make_unique<baphy::glh::VecBuffer<Vertex>>(6 * 20);

    vao = baphy::glh::create_vertex_array_from_bindings(
        shader,
        {{"aPos", 3, GL_FLOAT, false, offsetof(Vertex, pos)},
         {"aColor", 3, GL_FLOAT, false, offsetof(Vertex, color)}});

    glVertexArrayVertexBuffer(vao, 0, vbo->id, 0, sizeof(Vertex));
  }

  baphy::Color clear_color() override { return baphy::rgb(128, 64, 32); }

  void draw() override {
    vbo->sync();

    glm::mat4 projection = glm::ortho(
        0.0f,
        static_cast<float>(window.pixel_w()),
        static_cast<float>(window.pixel_h()),
        0.0f,
        1.0f,
        -1.0f);

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(projection));

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES,
                 static_cast<GLint>(vbo->front()),
                 static_cast<GLsizei>(vbo->size()));
  }

  void keyboard_callback(const baphy::KeyboardEvent &e) override {
    if (e.key == baphy::Key::Escape && e.action == baphy::Action::Up)
      window.set_should_close(true);
  }

  void mouse_button_callback(const baphy::MouseButtonEvent &e) override {
    if (e.button == baphy::Button::Left && e.action == baphy::Action::Down) {
      const auto left = e.x - 25.0f;
      const auto right = e.x + 25.0f;
      const auto top = e.y - 25.0f;
      const auto bottom = e.y + 25.0f;

      vbo->extend(std::array{
          Vertex{glm::vec3(left, top, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
          Vertex{glm::vec3(right, top, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
          Vertex{glm::vec3(left, bottom, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
          Vertex{glm::vec3(right, top, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
          Vertex{glm::vec3(right, bottom, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
          Vertex{glm::vec3(left, bottom, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)},
      });
    }
  }
};

int main(int, char *[]) {
  return baphy::run<Example>(
      {.title = "Example", .size = {1280, 960}, .resizable = true});
}
