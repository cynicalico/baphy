#include "baphy/baphy.hpp"

class Example final : public baphy::Application {
public:
  baphy::Window &window;
  baphy::Painter &painter;

  std::vector<std::pair<glm::vec2, baphy::RGBA>> rects;

  explicit Example(baphy::Runner &runner)
      : Application(runner),
        window(*runner.window),
        painter(*runner.painter) {}

  baphy::Color clear_color() override { return baphy::rgb(16, 16, 16); }

  void draw() override {
    for (const auto &[p, color]: rects)
      painter.rect(p, {50.0f, 50.0f}, color);
  }

  void keyboard_callback(const baphy::KeyboardEvent &e) override {
    if (e.key == baphy::Key::Escape && e.action == baphy::Action::Up)
      window.set_should_close(true);
  }

  void mouse_button_callback(const baphy::MouseButtonEvent &e) override {
    if (e.button == baphy::Button::Left && e.action == baphy::Action::Down) {
      const auto r = clero::rng(clero::int_range{0, 255});
      const auto g = clero::rng(clero::int_range{0, 255});
      const auto b = clero::rng(clero::int_range{0, 255});

      rects.emplace_back(std::make_pair<glm::vec2, baphy::RGBA>(
          {e.x - 25.0f, e.y - 25.0f}, baphy::rgb(r, g, b)));
    }
  }
};

int main(int, char *[]) {
  return baphy::run<Example>(
      {.title = "Example", .size = {1280, 960}, .resizable = true});
}
