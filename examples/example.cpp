#include "baphy/baphy.hpp"

namespace {
class Example final : public baphy::Application {
public:
    baphy::Window &window;
    baphy::Painter &painter;

    std::unique_ptr<baphy::Texture> texture{nullptr};

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

    texture = std::make_unique<baphy::Texture>("examples/assets/bulbasaur.png");
}

baphy::Color Example::clear_color() {
    return baphy::rgb(16, 16, 16);
}

void Example::draw() {
    painter.tex_sub(
            *texture, window.center() - texture->size_f() / 4.0f, std::nullopt, {0.0f, 0.0f}, texture->size_f() / 2.0f);
}

void Example::keyboard_callback(const baphy::KeyboardEvent &e) {
    if (e.key == baphy::Key::Escape)
        window.set_should_close(true);
}

int main(int, char *[]) {
    return baphy::run<Example>({.title = "Example", .size = {960, 720}, .resizable = false});
}
