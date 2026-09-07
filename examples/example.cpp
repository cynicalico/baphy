#include "baphy/baphy.hpp"

class Example : public baphy::Application {
public:
  Example() = default;

  void update(double dt) override {}

  void draw() override {
    glClearColor(0.2f, 0.1f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void key_callback(const baphy::Key key,
                    const baphy::Action action,
                    const baphy::ModFlags mods) override {
    if (key == baphy::Key::Escape && action == baphy::Action::Release)
      runner.window->set_should_close(true);
  }
};

int main(int, char *[]) {
  const auto window_opts = baphy::WindowOpts{
      .title = "Example", .size = {1280, 720}, .resizable = false};

  try {
    baphy::Runner::instance().run<Example>(window_opts);
  } catch (std::runtime_error &e) {
    BAPHY_LOG_ERROR("Runtime error: {}", e.what());
    return 1;
  }

  return 0;
}
