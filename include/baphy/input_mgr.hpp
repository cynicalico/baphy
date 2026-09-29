#ifndef BAPHY_INPUT_MGR_HPP
#define BAPHY_INPUT_MGR_HPP

#include <glm/glm.hpp>
#include <nexus/nexus.hpp>
#include <optional>
#include <unordered_map>
#include "baphy/event/all.hpp"

namespace baphy {
class Runner;

class InputMgr {
public:
  explicit InputMgr(Runner &runner);
  ~InputMgr();

  InputMgr(const InputMgr &) = delete;
  InputMgr &operator=(const InputMgr &) = delete;

  InputMgr(InputMgr &&) noexcept;
  InputMgr &operator=(InputMgr &&) noexcept = delete;

  void prev_propagate_();

  [[nodiscard]] glm::vec2 mouse_pos() const { return mouse.pos; }
  [[nodiscard]] glm::vec2 mouse_delta() const { return mouse.delta; }
  [[nodiscard]] glm::vec2 mouse_prev_pos() const { return mouse.prev_pos; }
  [[nodiscard]] glm::vec2 mouse_wheel() const { return mouse.wheel; }

  [[nodiscard]] bool button_pressed(Button button) const;
  [[nodiscard]] bool button_released(Button button) const;
  [[nodiscard]] bool button_down(Button button) const;

  [[nodiscard]] bool key_pressed(Key key) const;
  [[nodiscard]] bool key_released(Key key) const;
  [[nodiscard]] bool key_down(Key key) const;

  [[nodiscard]] bool scancode_pressed(Scancode scancode) const;
  [[nodiscard]] bool scancode_released(Scancode scancode) const;
  [[nodiscard]] bool scancode_down(Scancode scancode) const;

private:
  Runner &runner_;
  std::optional<nexus::ID> callback_id_{std::nullopt};

  struct {
    glm::vec2 pos{0, 0};
    glm::vec2 delta{0, 0};
    glm::vec2 prev_pos{0, 0};
    glm::vec2 wheel{0, 0};
    std::unordered_map<Button, bool> buttons_state{};
    std::unordered_map<Button, bool> buttons_prev_state{};
  } mouse{};

  struct {
    std::unordered_map<Key, bool> keys_state{};
    std::unordered_map<Key, bool> keys_prev_state{};
    std::unordered_map<Scancode, bool> scancodes_state{};
    std::unordered_map<Scancode, bool> scancodes_prev_state{};
  } keyboard{};

  void register_callbacks_();

  void keyboard_callback_(const KeyboardEvent &e);
  void mouse_button_callback_(const MouseButtonEvent &e);
  void mouse_motion_callback_(const MouseMotionEvent &e);
  void mouse_wheel_callback_(const MouseWheelEvent &e);
};
} // namespace baphy

#endif // BAPHY_INPUT_MGR_HPP
