#ifndef BAPHY_BAPHY_HPP
#define BAPHY_BAPHY_HPP

#define BAPHY_VERSION_MAJOR 0
#define BAPHY_VERSION_MINOR 1
#define BAPHY_VERSION_PATCH 0

#include "baphy/log.hpp"
#include "baphy/runner.hpp"
#include "baphy/window.hpp"

namespace baphy {
const char *version();
}

#include <fmt/format.h>
#include <glad/gl.h>

#endif // BAPHY_BAPHY_HPP
