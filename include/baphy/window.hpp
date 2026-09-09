#ifndef BAPHY_WINDOW_HPP
#define BAPHY_WINDOW_HPP

#include <GLFW/glfw3.h>
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

  [[nodiscard]] GLFWwindow *handle() const;

  [[nodiscard]] bool should_close() const;
  void set_should_close(bool should_close);

  [[nodiscard]] glm::ivec2 size() const;
  [[nodiscard]] int w() const;
  [[nodiscard]] int h() const;

  [[nodiscard]] glm::mat4 ortho_projection() const;

  void swap_buffers();

private:
  GLFWwindow *handle_;
};
} // namespace baphy

#endif // BAPHY_WINDOW_HPP
