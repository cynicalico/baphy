#include "baphy/baphy.hpp"

class Example : public baphy::Application {
public:
  Example() {
    runner.nexus->subscribe<baphy::KeyEvent>(callback_id_, [&](const auto *p) {
      if (p->key == baphy::Key::Escape && p->action == baphy::Action::Release)
        baphy::Runner::instance().window->set_should_close(true);
    });
  }

  void update(double dt) override {}

  void draw() override {
    glClearColor(0.2f, 0.1f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
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
