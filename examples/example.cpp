#include "baphy/baphy.hpp"

namespace {
class Example final : public baphy::Application {
public:
    baphy::Window &window;
    baphy::Painter &painter;

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
      painter(*runner.painter) {
    runner.show_fps = true;
    window.set_swap_interval(baphy::SwapInterval::Adaptive);

    font = std::make_unique<baphy::CP437_Font>(painter, "examples/assets/chunky_8x8.png", glm::vec2{8, 8});
}

baphy::Color Example::clear_color() {
    return baphy::rgb(16, 16, 16);
}

void Example::draw() {
    constexpr auto str = "Hello, world!";
    const auto bounds = font->calc_bounds(str, 3.0f);
    font->draw(str, window.center() - bounds / 2.0f, 3.0f);
}

void Example::keyboard_callback(const baphy::KeyboardEvent &e) {
    if (e.key == baphy::Key::Escape)
        window.set_should_close(true);
}

int main(int, char *[]) {
    return baphy::run<Example>({.title = "Example", .size = {960, 720}, .resizable = false});
}
