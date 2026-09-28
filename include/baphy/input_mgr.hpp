#ifndef BAPHY_INPUT_MGR_HPP
#define BAPHY_INPUT_MGR_HPP

#include <glm/glm.hpp>
#include <nexus/nexus.hpp>
#include <optional>
#include "baphy/event/all.hpp"

namespace baphy {
class Runner;

class InputMgr {
public:
  struct {
    float x{0};
    float y{0};
    float dx{0};
    float dy{0};
    float px{0};
    float py{0};
    glm::dvec2 wheel{0, 0};
  } mouse{};

  explicit InputMgr(Runner &runner);
  ~InputMgr();

  InputMgr(const InputMgr &) = delete;
  InputMgr &operator=(const InputMgr &) = delete;

  InputMgr(InputMgr &&) noexcept;
  InputMgr &operator=(InputMgr &&) noexcept = delete;

  void update_(double dt);

private:
  Runner &runner_;
  std::optional<nexus::ID> callback_id_{std::nullopt};

  void register_callbacks_();

  void keyboard_callback_(const KeyboardEvent &e);
  void mouse_button_callback_(const MouseButtonEvent &e);
  void mouse_motion_callback_(const MouseMotionEvent &e);
  void mouse_wheel_callback_(const MouseWheelEvent &e);
};
} // namespace baphy

#endif // BAPHY_INPUT_MGR_HPP
