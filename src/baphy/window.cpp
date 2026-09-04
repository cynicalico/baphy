#include "baphy/window.hpp"

#include <fmt/format.h>
#include <utility>

baphy::Window::Window() {
  window_ = glfwCreateWindow(800, 600, "example", nullptr, nullptr);
  if (!window_) {
    const char *description;
    auto code = glfwGetError(&description);
    throw std::runtime_error(fmt::format(
        "Failed to create GLFW window! Error {}: {}", code, description));
  }
}

baphy::Window::~Window() {
  if (window_)
    glfwDestroyWindow(window_);
}

baphy::Window::Window(Window &&other) noexcept
    : window_(std::exchange(other.window_, nullptr)) {}

baphy::Window &baphy::Window::operator=(Window &&other) noexcept {
  std::swap(window_, other.window_);
  return *this;
}

GLFWwindow *baphy::Window::raw() const {
  return window_;
}

bool baphy::Window::should_close() const {
  return glfwWindowShouldClose(window_) == GLFW_TRUE;
}

void baphy::Window::swap_buffers() {
  glfwSwapBuffers(window_);
}
