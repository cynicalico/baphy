#include "baphy/baphy.hpp"

class Example final : public baphy::Application {
public:
  baphy::Window &window;
  baphy::InputMgr &input;
  baphy::TimerMgr &timers;
  baphy::Painter &painter;

  float theta{0.0f};
  std::array<float, 4> p_thetas{0.0f, 0.0f, 0.0f, 0.0f};

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
    theta += 20.0f * dt;
    p_thetas[0] += 30.0f * dt;
    p_thetas[1] += 40.0f * dt;
    p_thetas[2] += 50.0f * dt;
    p_thetas[3] += 60.0f * dt;

    if (input.key_pressed(baphy::Key::Escape))
      window.set_should_close(true);
  }

  void draw() override {
    std::vector<glm::vec2> base_points;
    std::vector<glm::vec2> points;

    const auto r = std::min(window.w() / 3.0f, window.h() / 3.0f);

    for (const auto &[i, sub_t]: p_thetas | std::views::enumerate) {
      const auto t = glm::radians(theta + i * 90.0f);
      const auto bx = r * std::cos(t);
      const auto by = r * std::sin(t);

      base_points.emplace_back(window.center() + glm::vec2(bx, by));

      const auto dir = i % 2 == 0 ? 1 : -1;
      const auto x = (r / 3.0f) * std::cos(glm::radians(dir * sub_t));
      const auto y = (r / 3.0f) * std::sin(glm::radians(dir * sub_t));

      points.emplace_back(base_points[i] + glm::vec2(x, y));
    }

    for (const auto &p: base_points)
      painter.fill_circle(p, 9.0, baphy::rgb(255, 0, 0));
    for (const auto &p: points)
      painter.fill_circle(p, 7.0, baphy::rgb(0, 255, 0));

    painter.polyline(
        points, 5.0, true, baphy::LineJoin::round, baphy::rgb(255, 255, 255));
  }
};

int main(int, char *[]) {
  return baphy::run<Example>(
      {.title = "Example", .size = {960, 720}, .resizable = false});
}
