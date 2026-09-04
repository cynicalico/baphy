#ifndef BAPHY_RUNNER_HPP
#define BAPHY_RUNNER_HPP

#include <imgui.h>
#include <memory>
#include <nexus/nexus.hpp>
#include "baphy/window.hpp"

namespace baphy {
class Runner {
public:
  std::unique_ptr<Window> window{nullptr};
  std::unique_ptr<nexus::Nexus> nexus{nullptr};

  Runner(const Runner &) = delete;
  Runner &operator=(const Runner &) = delete;

  Runner(Runner &&) noexcept = delete;
  Runner &operator=(Runner &&) noexcept = delete;

  static Runner &instance();

  void poll_events();

private:
  ImGuiContext *imgui_ctx_{nullptr};

  Runner();
  ~Runner();
};
} // namespace baphy

#endif // BAPHY_RUNNER_HPP
