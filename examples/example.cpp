#include "baphy/baphy.hpp"

class Example final : public baphy::Application {
public:
  baphy::Window &window;
  baphy::InputMgr &input;
  baphy::TimerMgr &timers;
  baphy::Painter &painter;

  float theta{0.0f};

  explicit Example(baphy::Runner &runner)
      : Application(runner),
        window(*runner.window),
        input(*runner.input),
        timers(*runner.timers),
        painter(*runner.painter) {
    runner.show_fps = true;

    window.set_swap_interval(baphy::SwapInterval::Adaptive);
  }

  baphy::Color clear_color() override { return baphy::rgb(16, 16, 16); }

  void update(const double dt) override {
    theta += 45.0 * dt;

    if (input.key_pressed(baphy::Key::Escape))
      window.set_should_close(true);
  }

  void draw() override {
    const auto x = 10.0f + 25.0f * std::abs(std::cos(glm::radians(3 * theta)));
    const auto y = 10.0f + 25.0f * std::abs(std::sin(glm::radians(5 * theta)));
    painter.ellipse(window.center(), {x, y}, baphy::rgb(0, 255, 255));
  }
};

int main(int, char *[]) {
  return baphy::run<Example>(
      {.title = "Example", .size = {480, 360}, .resizable = false});
}
