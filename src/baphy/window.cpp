#include "baphy/window.hpp"

#include <fmt/format.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>

baphy::Window::Window(const WindowOpts &opts) {
  SDL_WindowFlags flags = SDL_WINDOW_OPENGL;
  if (opts.resizable)
    flags |= SDL_WINDOW_RESIZABLE;

  handle_ =
      SDL_CreateWindow(opts.title.c_str(), opts.size.x, opts.size.y, flags);
  if (!handle_) {
    throw std::runtime_error(
        fmt::format("Failed to create SDL3 window! {}", SDL_GetError()));
  }

  context_ = SDL_GL_CreateContext(handle_);
  if (!context_) {
    throw std::runtime_error(
        fmt::format("Failed to create SDL3 context! {}", SDL_GetError()));
  }
}

baphy::Window::~Window() {
  if (handle_) {
    SDL_GL_DestroyContext(context_);
    context_ = nullptr;

    SDL_DestroyWindow(handle_);
    handle_ = nullptr;
  }
}

baphy::Window::Window(Window &&other) noexcept
    : handle_(std::exchange(other.handle_, nullptr)),
      context_(std::exchange(other.context_, nullptr)) {}

baphy::Window &baphy::Window::operator=(Window &&other) noexcept {
  std::swap(handle_, other.handle_);
  std::swap(context_, other.context_);
  return *this;
}

SDL_Window *baphy::Window::handle() const {
  return handle_;
}

SDL_GLContext baphy::Window::context() const {
  return context_;
}

bool baphy::Window::should_close() const {
  return should_close_;
}

void baphy::Window::set_should_close(const bool should_close) {
  should_close_ = should_close;
}

void baphy::Window::set_size(glm::ivec2 size) {
  SDL_SetWindowSize(handle_, size.x, size.y);
}

void baphy::Window::set_w(int w) {
  set_size({w, h()});
}

void baphy::Window::set_h(int h) {
  set_size({w(), h});
}

glm::ivec2 baphy::Window::size() const {
  int w;
  int h;
  SDL_GetWindowSize(handle_, &w, &h);

  return {w, h};
}

int baphy::Window::w() const {
  int w;
  SDL_GetWindowSize(handle_, &w, nullptr);

  return w;
}

int baphy::Window::h() const {
  int h;
  SDL_GetWindowSize(handle_, nullptr, &h);

  return h;
}

void baphy::Window::set_resizable(bool resizable) {
  SDL_SetWindowResizable(handle_, resizable);
}

bool baphy::Window::resizable() const {
  return SDL_GetWindowFlags(handle_) & SDL_WINDOW_RESIZABLE;
}

bool baphy::Window::set_swap_interval(SwapInterval interval) {
  int sdl_interval;

  switch (interval) {
  case SwapInterval::Immediate:
    sdl_interval = 0;
    break;
  case SwapInterval::VSync:
    sdl_interval = 1;
    break;
  case SwapInterval::Adaptive:
    sdl_interval = -1;
    break;
  }

  return SDL_GL_SetSwapInterval(sdl_interval);
}

baphy::SwapInterval baphy::Window::swap_interval() const {
  int interval;
  SDL_GL_GetSwapInterval(&interval); // TODO probably should error check

  switch (interval) {
  case 0:
    return SwapInterval::Immediate;
  case 1:
    return SwapInterval::VSync;
  case -1:
    return SwapInterval::Adaptive;
  default:
    std::unreachable();
  }
}

glm::mat4 baphy::Window::ortho_projection() const {
  return glm::ortho(
      0.0f, static_cast<float>(w()), static_cast<float>(h()), 0.0f);
}

void baphy::Window::swap_buffers() {
  SDL_GL_SwapWindow(handle_);
}
