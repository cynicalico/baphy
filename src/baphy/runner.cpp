#include "baphy/runner.hpp"

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glad/gl.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>
#include "baphy/event/all.hpp"
#include "baphy/log.hpp"

static void char_callback(GLFWwindow *window, unsigned int codepoint);
static void cursor_enter_callback(GLFWwindow *window, int entered);
static void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
static void drop_callback(GLFWwindow *, int count, const char **paths);
static void
mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
static void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
static void scroll_callback(GLFWwindow *, double xoffset, double yoffset);

baphy::Runner::Runner() {
  nexus = std::make_unique<nexus::Nexus>();
}

baphy::Runner::~Runner() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext(imgui_ctx_);
  imgui_ctx_ = nullptr;

  window.reset();
  glfwTerminate();

  nexus.reset();
}

void baphy::Runner::open_window_(const WindowOpts &opts) {
  if (!glfwInit()) {
    const char *description;
    auto code = glfwGetError(&description);
    throw std::runtime_error(fmt::format(
        "Failed to initialize GLFW! Error {}: {}", code, description));
  }
  BAPHY_LOG_DEBUG("GLFW v{}.{}.{}",
                  GLFW_VERSION_MAJOR,
                  GLFW_VERSION_MINOR,
                  GLFW_VERSION_REVISION);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
  glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);
#endif
  window = std::make_unique<Window>(opts);

  glfwSetCharCallback(window->handle(), char_callback);
  glfwSetCursorPosCallback(window->handle(), cursor_pos_callback);
  glfwSetCursorEnterCallback(window->handle(), cursor_enter_callback);
  glfwSetDropCallback(window->handle(), drop_callback);
  glfwSetKeyCallback(window->handle(), key_callback);
  glfwSetMouseButtonCallback(window->handle(), mouse_button_callback);
  glfwSetScrollCallback(window->handle(), scroll_callback);

  glfwMakeContextCurrent(window->handle());
  if (gladLoadGL(glfwGetProcAddress) == 0)
    throw std::runtime_error("Failed to initialize GLAD!");
  BAPHY_LOG_DEBUG("OpenGL v{}",
                  reinterpret_cast<const char *>(glGetString(GL_VERSION)));
  BAPHY_LOG_DEBUG("OpenGL renderer: {}",
                  reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
  BAPHY_LOG_DEBUG("OpenGL vendor: {}",
                  reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
}

void baphy::Runner::initialize_imgui_() {
  IMGUI_CHECKVERSION();
  imgui_ctx_ = ImGui::CreateContext();
  if (!imgui_ctx_)
    throw std::runtime_error("Failed to initialize ImGui!");

  ImGui::StyleColorsDark();
  ImGui::GetIO().IniFilename = nullptr;

  if (!ImGui_ImplGlfw_InitForOpenGL(window->handle(), false))
    throw std::runtime_error("Failed to initialize ImGui GLFW backend!");
  if (!ImGui_ImplOpenGL3_Init("#version 410"))
    throw std::runtime_error("Failed to initialize ImGui OpenGL3 backend!");
  BAPHY_LOG_DEBUG("ImGui v{}", ImGui::GetVersion());
}

baphy::Runner &baphy::Runner::instance() {
  static Runner instance;
  return instance;
}

void baphy::Runner::run_() {
  while (!window->should_close()) {
    glfwPollEvents();
    app_->update(0.0);

    app_->draw();

    window->swap_buffers();
  }
}

void char_callback(GLFWwindow *window, unsigned int codepoint) {
  ImGui_ImplGlfw_CharCallback(window, codepoint);

  if (!ImGui::GetIO().WantCaptureKeyboard)
    baphy::Runner::instance().nexus->publish<baphy::CharEvent>(codepoint);
}

void cursor_enter_callback(GLFWwindow *window, const int entered) {
  ImGui_ImplGlfw_CursorEnterCallback(window, entered);

  if (!ImGui::GetIO().WantCaptureMouse) {
    baphy::Runner::instance().nexus->publish<baphy::CursorEnterEvent>(
        entered != 0);
  }
}

void cursor_pos_callback(GLFWwindow *window,
                         const double xpos,
                         const double ypos) {
  ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

  if (!ImGui::GetIO().WantCaptureMouse)
    baphy::Runner::instance().nexus->publish<baphy::CursorPosEvent>(xpos, ypos);
}

void drop_callback(GLFWwindow *, const int count, const char **paths) {
  std::vector<std::string> owned_paths;
  owned_paths.reserve(static_cast<std::size_t>(count));
  for (int i = 0; i < count; ++i)
    owned_paths.emplace_back(paths[i]);

  baphy::Runner::instance().nexus->publish<baphy::DropEvent>(
      std::move(owned_paths));
}

void mouse_button_callback(
    GLFWwindow *window, const int button, const int action, const int mods) {
  ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

  if (!ImGui::GetIO().WantCaptureMouse) {
    baphy::Runner::instance().nexus->publish<baphy::MouseButtonEvent>(
        static_cast<baphy::Button>(button),
        static_cast<baphy::Action>(action),
        static_cast<baphy::ModFlags>(mods));
  }
}

static void key_callback(GLFWwindow *window,
                         const int key,
                         const int scancode,
                         const int action,
                         const int mods) {
  ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

  if (!ImGui::GetIO().WantCaptureKeyboard) {
    baphy::Runner::instance().nexus->publish<baphy::KeyEvent>(
        static_cast<baphy::Key>(key),
        scancode,
        static_cast<baphy::Action>(action),
        static_cast<baphy::ModFlags>(mods));
  }
}

void scroll_callback(GLFWwindow *window,
                     const double xoffset,
                     const double yoffset) {
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

  if (!ImGui::GetIO().WantCaptureMouse) {
    baphy::Runner::instance().nexus->publish<baphy::ScrollEvent>(
        xoffset, yoffset);
  }
}
