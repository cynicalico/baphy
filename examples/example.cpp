#include "baphy/baphy.hpp"

class Example final : public baphy::Application {
public:
  baphy::Window &window;
  baphy::Painter &painter;

  double theta{0.0};

  explicit Example(baphy::Runner &runner)
      : Application(runner),
        window(*runner.window),
        painter(*runner.painter) {
    window.set_swap_interval(baphy::SwapInterval::Adaptive);
  }

  baphy::Color clear_color() override { return baphy::rgb(16, 16, 16); }

  void update(const double dt) override { theta += 90.0 * dt; }

  void draw() override {
    const auto x =
        (window.pixel_w() / 2.0) + (50.0 * std::cos(glm::radians(theta)));
    const auto y =
        (window.pixel_h() / 2.0) + (50.0 * std::sin(glm::radians(theta)));

    painter.square({x - 25.0, y - 25.0}, 50.0, baphy::rgb(255, 255, 255));

    painter.line({window.pixel_w() / 2.0, window.pixel_h() / 2.0},
                 {x, y},
                 baphy::rgb(255, 0, 255));
  }

  void keyboard_callback(const baphy::KeyboardEvent &e) override {
    if (e.key == baphy::Key::Escape && e.action == baphy::Action::Up)
      window.set_should_close(true);
  }
};

int main(int, char *[]) {
  return baphy::run<Example>(
      {.title = "Example", .size = {480, 360}, .resizable = false});
}
