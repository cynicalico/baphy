#include "baphy/runner.hpp"

#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include <stdexcept>

baphy::Runner::Runner() {
  if (!glfwInit()) {
    const char *description;
    auto code = glfwGetError(&description);
    throw std::runtime_error(
        fmt::format("Failed to initialize GLFW! Error {}: {}", code, description));
  }

  window = std::make_unique<Window>();
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
