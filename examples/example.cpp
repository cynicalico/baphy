#include "baphy/baphy.hpp"
#include "glm/gtc/type_ptr.hpp"

class Example final : public baphy::Application {
public:
  explicit Example(baphy::Runner &runner)
      : Application(runner) {}

  void update(double dt) override {}

  void draw() override {
    glClearColor(0.2f, 0.1f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    if (ImGui::Begin("hello",
                     nullptr,
                     ImGuiWindowFlags_AlwaysAutoResize |
                         ImGuiWindowFlags_NoDecoration)) {
      ImGui::Text("%.2f fps", ImGui::GetIO().Framerate);
    }
    ImGui::End();

    if (ImGui::Begin("Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
      static glm::ivec2 size;
      size = runner.window->size();
      if (ImGui::InputInt2("Size",
                           glm::value_ptr(size),
                           ImGuiInputTextFlags_EnterReturnsTrue)) {
        runner.window->set_size(size);
      }

      static bool resizable;
      resizable = runner.window->resizable();
      if (ImGui::Checkbox("Resizable", &resizable))
        runner.window->set_resizable(resizable);
    }
    ImGui::End();
  }

  void keyboard_callback(const baphy::KeyboardEvent &e) override {
    if (e.key == baphy::Key::Escape && e.action == baphy::Action::Up)
      runner.window->set_should_close(true);
  }
};

int main(int, char *[]) {
  const auto window_opts = baphy::WindowOpts{
      .title = "Example", .size = {1280, 720}, .resizable = true};

  try {
    baphy::Runner r{};
    r.run<Example>(window_opts);
  } catch (std::runtime_error &e) {
    BAPHY_LOG_ERROR("Runtime error: {}", e.what());
    return 1;
  }

  return 0;
}
