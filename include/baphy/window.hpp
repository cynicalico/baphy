#ifndef BAPHY_WINDOW_HPP
#define BAPHY_WINDOW_HPP

#include <GLFW/glfw3.h>

namespace baphy {
class Window {
public:
  Window();
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  Window(Window &&) noexcept;
  Window &operator=(Window &&) noexcept;

  [[nodiscard]] GLFWwindow *raw() const;

  [[nodiscard]] bool should_close() const;
  void swap_buffers();

private:
  GLFWwindow *window_;
};
} // namespace baphy

#endif // BAPHY_WINDOW_HPP
