#ifndef BAPHY_WINDOW_HPP
#define BAPHY_WINDOW_HPP

#include <memory>

namespace baphy {
class Window {
public:
  Window();
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  Window(Window &&) noexcept;
  Window &operator=(Window &&) noexcept;

  bool should_close() const;
  void swap_buffers();

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
} // namespace baphy

#endif // BAPHY_WINDOW_HPP
