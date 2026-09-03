#include "baphy/window.hpp"

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <utility>

struct baphy::Window::Impl {
  GLFWwindow *window{nullptr};

  ~Impl();
};

baphy::Window::Impl::~Impl() {
  if (window) glfwDestroyWindow(window);
}

baphy::Window::Window()
    : impl_(std::make_unique<Impl>()) {
  impl_->window = glfwCreateWindow(800, 600, "example", nullptr, nullptr);
  if (!impl_->window) {
    const char *description;
    auto code = glfwGetError(&description);
    throw std::runtime_error(
        fmt::format("Failed to create GLFW window! Error {}: {}", code, description));
  }
}

baphy::Window::~Window() = default;

baphy::Window::Window(Window &&other) noexcept
    : impl_(std::exchange(other.impl_, nullptr)) {}

baphy::Window &baphy::Window::operator=(Window &&other) noexcept {
  std::swap(impl_, other.impl_);
  return *this;
}

bool baphy::Window::should_close() const {
  return glfwWindowShouldClose(impl_->window) == GLFW_TRUE;
}

void baphy::Window::swap_buffers() {
  glfwSwapBuffers(impl_->window);
}
