#ifndef BAPHY_EVENT_KEYBOARD_HPP
#define BAPHY_EVENT_KEYBOARD_HPP

#include "baphy/event/common.hpp"

namespace baphy {
struct KeyboardEvent {
  std::uint64_t timestamp;
  Action action;
  Scancode scancode;
  Key key;
  ModFlags mods;
  std::uint16_t raw;
};
} // namespace baphy

#endif // BAPHY_EVENT_KEYBOARD_HPP
