#ifndef BAPHY_RUNNER_HPP
#define BAPHY_RUNNER_HPP

#include <imgui.h>
#include <memory>
#include <nexus/nexus.hpp>
#include <type_traits>
#include "baphy/application.hpp"
#include "baphy/window.hpp"

namespace baphy {
class Runner {
public:
  std::unique_ptr<nexus::Nexus> nexus{nullptr};
  std::unique_ptr<Window> window{nullptr};

  Runner(const Runner &) = delete;
  Runner &operator=(const Runner &) = delete;

  Runner(Runner &&) noexcept = delete;
  Runner &operator=(Runner &&) noexcept = delete;

  static Runner &instance();

  template<typename T>
    requires std::is_base_of_v<Application, T>
  void run(const WindowOpts &opts);

private:
  ImGuiContext *imgui_ctx_{nullptr};
  Application *app_{nullptr};

  Runner();
  ~Runner();

  void open_window_(const WindowOpts &opts);
  void initialize_imgui_();

  void run_();
};

template<typename T>
  requires std::is_base_of_v<Application, T>
void Runner::run(const WindowOpts &opts) {
  open_window_(opts);
  initialize_imgui_();

  app_ = new T();

  run_();

  delete app_;
  app_ = nullptr;
}
} // namespace baphy

#endif // BAPHY_RUNNER_HPP
