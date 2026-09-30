#include "baphy/baphy.hpp"

class Example final : public baphy::Application {
public:
  baphy::Window &window;
  baphy::InputMgr &input;
  baphy::TimerMgr &timers;
  baphy::Painter &painter;

  double theta{0.0};

  explicit Example(baphy::Runner &runner)
      : Application(runner),
        window(*runner.window),
        input(*runner.input),
        timers(*runner.timers),
        painter(*runner.painter) {
    runner.show_fps = true;

    window.set_swap_interval(baphy::SwapInterval::Adaptive);

    timers.every(1.0, [&] {
      BAPHY_LOG_INFO("test 2");
    });
  }

  baphy::Color clear_color() override { return baphy::rgb(16, 16, 16); }

  void update(const double dt) override {
    theta += 90.0 * dt;

    if (input.key_pressed(baphy::Key::Escape))
      window.set_should_close(true);

    if (input.button_pressed(baphy::Button::Left)) {
      timers.after(1.0, [&] {
        BAPHY_LOG_INFO("test");
      });
    }
  }

  void draw() override {
    const auto center = window.center();

    const auto x = center.x + 50.0 * std::cos(glm::radians(theta));
    const auto y = center.y + 50.0 * std::sin(glm::radians(theta));
    painter.square({x - 25.0, y - 25.0}, 50.0, baphy::rgb(0, 255, 255));
  }
};

int main(int, char *[]) {
  return baphy::run<Example>(
      {.title = "Example", .size = {480, 360}, .resizable = false});
}
