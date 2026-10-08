#include "baphy/baphy.hpp"

class Example final : public baphy::Application {
public:
    baphy::Window &window;
    baphy::InputMgr &input;
    baphy::TimerMgr &timers;
    baphy::Painter &painter;

    std::unique_ptr<baphy::glh::Texture> texture{nullptr};
    float theta{0.0f};

    explicit Example(baphy::Runner &runner)
        : Application(runner),
          window(*runner.window),
          input(*runner.input),
          timers(*runner.timers),
          painter(*runner.painter) {
        runner.show_fps = true;
        window.set_swap_interval(baphy::SwapInterval::Adaptive);

        texture = baphy::glh::load_texture("examples/assets/bulbasaur.png");
    }

    baphy::Color clear_color() override { return baphy::rgb(16, 16, 16); }

    void update(const double dt) override {
        theta += glm::radians(static_cast<float>(dt) * 90.0f);

        if (input.key_pressed(baphy::Key::Escape))
            window.set_should_close(true);
    }

    void draw() override {
        const float scale = 3.0f;
        const auto p = window.center() + glm::vec2{std::cos(theta), std::sin(theta)} * (scale * 16.0f);

        painter.fill_circle(p, scale * 32.0f, baphy::rgb(255, 0, 0));

        painter.tex(*texture,
                    window.center() - scale * glm::vec2(texture->w, texture->h) / 2.0f,
                    scale * glm::vec2{texture->w, texture->h});

        painter.line({0, 0}, p, 32.0f, baphy::rgb(255, 0, 0));
        painter.line({window.w(), window.h()}, p, 32.0f, baphy::rgba(0, 255, 0, 128));
    }
};

int main(int, char *[]) {
    return baphy::run<Example>({.title = "Example", .size = {960, 720}, .resizable = false});
}
