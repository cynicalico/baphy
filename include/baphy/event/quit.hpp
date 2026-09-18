#ifndef BAPHY_EVENT_QUIT_HPP
#define BAPHY_EVENT_QUIT_HPP

#include "baphy/event/common.hpp"

namespace baphy {
struct QuitEvent {
  std::uint64_t timestamp;
};
} // namespace baphy

#endif // BAPHY_EVENT_QUIT_HPP
