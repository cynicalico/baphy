#ifndef BAPHY_WINDOW_HPP
#define BAPHY_WINDOW_HPP

#include <SDL3/SDL.h>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <string>

namespace baphy {
struct WindowOpts {
  std::string title{"baphy Application"};
  glm::ivec2 size{800, 600};
  bool resizable{false};
};

class Window {
public:
  explicit Window(const WindowOpts &opts);
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  Window(Window &&) noexcept;
  Window &operator=(Window &&) noexcept;

  [[nodiscard]] SDL_Window *handle() const;
  [[nodiscard]] SDL_GLContext context() const;

  [[nodiscard]] bool should_close() const;
  void set_should_close(bool should_close);

  void set_size(glm::ivec2 size);
  void set_w(int w);
  void set_h(int h);
  [[nodiscard]] glm::ivec2 size() const;
  [[nodiscard]] int w() const;
  [[nodiscard]] int h() const;

  void set_resizable(bool resizable);
  [[nodiscard]] bool resizable() const;

  [[nodiscard]] glm::mat4 ortho_projection() const;

  void swap_buffers();

private:
  bool should_close_{false};

  SDL_Window *handle_{nullptr};
  SDL_GLContext context_{nullptr};
};
} // namespace baphy

#endif // BAPHY_WINDOW_HPP
