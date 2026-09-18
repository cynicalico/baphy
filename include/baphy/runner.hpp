#ifndef BAPHY_RUNNER_HPP
#define BAPHY_RUNNER_HPP

#include <imgui.h>
#include <memory>
#include <nexus/nexus.hpp>
#include <type_traits>
#include "baphy/application.hpp"
#include "baphy/util/time.hpp"
#include "baphy/window.hpp"
#include "implot.h"

namespace baphy {
class Runner {
public:
  FrameCounter<> frame_counter{};
  bool first_frame{true};

  std::unique_ptr<nexus::Nexus> nexus{nullptr};
  std::unique_ptr<Window> window{nullptr};

  Runner();
  ~Runner();

  Runner(const Runner &) = delete;
  Runner &operator=(const Runner &) = delete;

  Runner(Runner &&) noexcept = delete;
  Runner &operator=(Runner &&) noexcept = delete;

  template<typename T>
    requires std::is_base_of_v<Application, T>
  void run(const WindowOpts &opts);

private:
  ImGuiContext *imgui_ctx_{nullptr};
  ImPlotContext *implot_ctx_{nullptr};

  std::unique_ptr<Application> app_{nullptr};

  void open_window_(const WindowOpts &opts);
  void initialize_imgui_();

  void run_();
  void poll_events_();
  void draw_fps_overlay_();
};

template<typename T>
  requires std::is_base_of_v<Application, T>
void Runner::run(const WindowOpts &opts) {
  open_window_(opts);
  initialize_imgui_();

  app_ = std::make_unique<T>(*this);

  run_();

  app_.reset();
}
} // namespace baphy

#endif // BAPHY_RUNNER_HPP
