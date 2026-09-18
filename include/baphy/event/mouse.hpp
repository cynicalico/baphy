#ifndef BAPHY_EVENT_MOUSE_MOTION_HPP
#define BAPHY_EVENT_MOUSE_MOTION_HPP

#include "baphy/event/common.hpp"

namespace baphy {
struct MouseButtonEvent {
  std::uint64_t timestamp;
  Action action;
  Button button;
  std::uint8_t clicks;
  float x;
  float y;
};

struct MouseMotionEvent {
  std::uint64_t timestamp;
  ButtonFlags buttons;
  float x;
  float y;
  float dx;
  float dy;
};

struct MouseWheelEvent {
  std::uint64_t timestamp;
  float x;
  float y;
  WheelDirection direction;
  float mouse_x;
  float mouse_y;
  std::int32_t integer_x;
  std::int32_t integer_y;
};
} // namespace baphy

#endif // BAPHY_EVENT_MOUSE_MOTION_HPP
