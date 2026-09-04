#include "baphy/runner.hpp"

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glad/gl.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>
#include "baphy/event/all.hpp"
#include "baphy/log.hpp"

static void char_callback(GLFWwindow *, unsigned int codepoint);
static void cursor_enter_callback(GLFWwindow *, int entered);
static void cursor_pos_callback(GLFWwindow *, double xpos, double ypos);
static void drop_callback(GLFWwindow *, int count, const char **paths);
static void
mouse_button_callback(GLFWwindow *, int button, int action, int mods);
static void
key_callback(GLFWwindow *, int key, int scancode, int action, int mods);
static void scroll_callback(GLFWwindow *, double xoffset, double yoffset);

baphy::Runner::Runner() {
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
  window = std::make_unique<Window>();

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

  nexus = std::make_unique<nexus::Nexus>();
}

baphy::Runner::~Runner() {
  window.reset();
  glfwTerminate();
}

baphy::Runner &baphy::Runner::instance() {
  static Runner instance;
  return instance;
}

void baphy::Runner::poll_events() {
  glfwPollEvents();
}

void char_callback(GLFWwindow *, unsigned int codepoint) {
  baphy::Runner::instance().nexus->publish<baphy::CharEvent>(codepoint);
}

void cursor_enter_callback(GLFWwindow *, const int entered) {
  baphy::Runner::instance().nexus->publish<baphy::CursorEnterEvent>(
      entered != 0);
}

void cursor_pos_callback(GLFWwindow *, const double xpos, const double ypos) {
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

static void mouse_button_callback(
    GLFWwindow *, const int button, const int action, const int mods) {
  baphy::Runner::instance().nexus->publish<baphy::MouseButtonEvent>(
      static_cast<baphy::Button>(button),
      static_cast<baphy::Action>(action),
      static_cast<baphy::ModFlags>(mods));
}

static void key_callback(GLFWwindow *,
                         const int key,
                         const int scancode,
                         const int action,
                         const int mods) {
  baphy::Runner::instance().nexus->publish<baphy::KeyEvent>(
      static_cast<baphy::Key>(key),
      scancode,
      static_cast<baphy::Action>(action),
      static_cast<baphy::ModFlags>(mods));
}

void scroll_callback(GLFWwindow *, const double xoffset, const double yoffset) {
  baphy::Runner::instance().nexus->publish<baphy::ScrollEvent>(
      xoffset, yoffset);
}
