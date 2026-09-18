#include "baphy/baphy.hpp"
#include "glm/gtc/type_ptr.hpp"

class Example final : public baphy::Application {
public:
  baphy::Window &window;

  explicit Example(baphy::Runner &runner)
      : Application(runner),
        window(*runner.window) {}

  baphy::Color clear_color() override { return baphy::rgb(128, 64, 32); }

  void update(double dt) override {}

  void draw() override {
    if (ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
      static baphy::SwapInterval selected_swap_interval;
      selected_swap_interval = window.swap_interval();

      constexpr const char *swap_interval_names[] = {
          "Immediate",
          "VSync",
          "Adaptive",
      };

      int selected_index = static_cast<int>(selected_swap_interval);

      if (ImGui::Combo("Swap Interval",
                       &selected_index,
                       swap_interval_names,
                       IM_ARRAYSIZE(swap_interval_names))) {
        selected_swap_interval =
            static_cast<baphy::SwapInterval>(selected_index);
        window.set_swap_interval(selected_swap_interval);
      }
    }
    ImGui::End();
  }

  void keyboard_callback(const baphy::KeyboardEvent &e) override {
    if (e.key == baphy::Key::Escape && e.action == baphy::Action::Up)
      window.set_should_close(true);
  }
};

int main(int, char *[]) {
  return baphy::run<Example>(
      {.title = "Example", .size = {1280, 960}, .resizable = true});
}
