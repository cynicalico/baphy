#ifndef BAPHY_EVENT_MOUSE_HPP
#define BAPHY_EVENT_MOUSE_HPP

#include "baphy/event/common.hpp"

namespace baphy {
struct MouseButtonEvent {
  Button button;
  Action action;
  ModFlags mods;
};
} // namespace baphy

#endif // BAPHY_EVENT_MOUSE_HPP
