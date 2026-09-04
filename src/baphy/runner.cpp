#include "baphy/runner.hpp"

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <glad/gl.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdexcept>
#include "baphy/log.hpp"

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
  glfwMakeContextCurrent(window->raw());

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

  if (!ImGui_ImplGlfw_InitForOpenGL(window->raw(), false))
    throw std::runtime_error("Failed to initialize ImGui GLFW backend!");
  if (!ImGui_ImplOpenGL3_Init("#version 410"))
    throw std::runtime_error("Failed to initialize ImGui OpenGL3 backend!");
  BAPHY_LOG_DEBUG("ImGui v{}", ImGui::GetVersion());
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
