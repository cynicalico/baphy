#include "baphy/baphy.hpp"

#define STRINGIFY(x) #x
#define EXPAND(x) STRINGIFY(x)

const char *baphy::version() {
  return EXPAND(BAPHY_VERSION_MAJOR) "." EXPAND(BAPHY_VERSION_MINOR) "." EXPAND(
      BAPHY_VERSION_PATCH);
}
