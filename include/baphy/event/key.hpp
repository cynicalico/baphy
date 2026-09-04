#ifndef BAPHY_EVENT_KEYBOARD_HPP
#define BAPHY_EVENT_KEYBOARD_HPP

#include "baphy/event/common.hpp"

namespace baphy {
struct KeyEvent {
  Key key;
  int scancode;
  Action action;
  ModFlags mods;
};
} // namespace baphy

#endif // BAPHY_EVENT_KEYBOARD_HPP
