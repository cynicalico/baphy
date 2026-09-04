#ifndef BAPHY_EVENT_DROP_HPP
#define BAPHY_EVENT_DROP_HPP

#include <string>
#include <vector>

namespace baphy {
struct DropEvent {
  std::vector<std::string> paths;
};
} // namespace baphy

#endif // BAPHY_EVENT_DROP_HPP
