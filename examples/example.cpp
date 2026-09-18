#include "baphy/baphy.hpp"
#include "glm/gtc/type_ptr.hpp"

class Example final : public baphy::Application {
public:
  baphy::Window &window;

  explicit Example(baphy::Runner &runner)
      : Application(runner),
        window(*runner.window) {}

  void update(double dt) override {}

  void draw() override {
    glClearColor(0.2f, 0.1f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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
