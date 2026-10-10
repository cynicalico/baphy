#include "baphy/baphy.hpp"

namespace {
class Example final : public baphy::Application {
public:
    baphy::Window &window;
    baphy::Painter &painter;
    baphy::InputMgr &input;

    std::unique_ptr<baphy::CP437_Font> font{nullptr};

    explicit Example(baphy::Runner &runner);

    baphy::Color clear_color() override;

    void draw() override;

    void keyboard_callback(const baphy::KeyboardEvent &e) override;
};
} // namespace

Example::Example(baphy::Runner &runner)
    : Application(runner),
      window(*runner.window),
      painter(*runner.painter),
      input(*runner.input) {
    runner.show_fps = true;
    window.set_swap_interval(baphy::SwapInterval::Adaptive);

    font = std::make_unique<baphy::CP437_Font>(painter, "examples/assets/chunky_8x8.png", glm::vec2{8, 8});
}

baphy::Color Example::clear_color() {
    return baphy::rgb(16, 16, 16);
}

void Example::draw() {
    font->draw(fmt::format("{} {}", input.mouse_pos().x, input.mouse_pos().y), {100, 200});

    painter.stroke_rect({0, 100}, {7, 7}, 3.0f, baphy::rgb(255, 255, 255));

    constexpr auto str = "Hello, world!";
    const auto bounds = font->calc_bounds(str, 3.0f);
    const auto pos = glm::round(input.mouse_pos()) - bounds / 2.0f;

    painter.fill_rect(pos, bounds, baphy::rgb(0, 0, 128));
    font->draw(str, pos, 3.0f);
    painter.stroke_rect(pos - glm::vec2{1.0f}, bounds + glm::vec2{2.0f}, 1.0, baphy::rgb(255, 255, 255));

    painter.stroke_circle({300, 300}, 50.0f, 1.0f, baphy::rgb(255, 255, 255));
}

void Example::keyboard_callback(const baphy::KeyboardEvent &e) {
    if (e.key == baphy::Key::Escape)
        window.set_should_close(true);
}

int main(int, char *[]) {
    return baphy::run<Example>({.title = "Example", .size = {960, 720}, .resizable = false});
}
