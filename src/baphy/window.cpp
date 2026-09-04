#include "baphy/window.hpp"

#include <fmt/format.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>

baphy::Window::Window() {
  handle_ = glfwCreateWindow(800, 600, "example", nullptr, nullptr);
  if (!handle_) {
    const char *description;
    auto code = glfwGetError(&description);
    throw std::runtime_error(fmt::format(
        "Failed to create GLFW window! Error {}: {}", code, description));
  }
}

baphy::Window::~Window() {
  if (handle_)
    glfwDestroyWindow(handle_);
}

baphy::Window::Window(Window &&other) noexcept
    : handle_(std::exchange(other.handle_, nullptr)) {}

baphy::Window &baphy::Window::operator=(Window &&other) noexcept {
  std::swap(handle_, other.handle_);
  return *this;
}

GLFWwindow *baphy::Window::handle() const {
  return handle_;
}

bool baphy::Window::should_close() const {
  return glfwWindowShouldClose(handle_) == GLFW_TRUE;
}

glm::ivec2 baphy::Window::size() const {
  int w;
  int h;
  glfwGetWindowSize(handle_, &w, &h);

  return {w, h};
}

int baphy::Window::w() const {
  int w;
  glfwGetWindowSize(handle_, &w, nullptr);

  return w;
}

int baphy::Window::h() const {
  int h;
  glfwGetWindowSize(handle_, nullptr, &h);

  return h;
}

glm::mat4 baphy::Window::ortho_projection() const {
  return glm::ortho(
      0.0f, static_cast<float>(w()), static_cast<float>(h()), 0.0f);
}

void baphy::Window::swap_buffers() {
  glfwSwapBuffers(handle_);
}
