#ifndef BAPHY_BAPHY_HPP
#define BAPHY_BAPHY_HPP

#define BAPHY_VERSION_MAJOR 0
#define BAPHY_VERSION_MINOR 1
#define BAPHY_VERSION_PATCH 0

#include "baphy/application.hpp"
#include "baphy/event/all.hpp"
#include "baphy/gl.hpp"
#include "baphy/glh/shader.hpp"
#include "baphy/log.hpp"
#include "baphy/runner.hpp"
#include "baphy/glh/glh.hpp"
#include "baphy/util/averagers.hpp"
#include "baphy/util/envvars.hpp"
#include "baphy/util/io.hpp"
#include "baphy/util/platform.hpp"
#include "baphy/util/time.hpp"
#include "baphy/window.hpp"

namespace baphy {
const char *version();

template<typename T>
  requires std::derived_from<T, Application>
int run(const WindowOpts &window_opts) {
  try {
    Runner().run<T>(window_opts);
  } catch (std::runtime_error &e) {
    BAPHY_LOG_ERROR("Runtime error: {}", e.what());
    return 1;
  }

  return 0;
}
} // namespace baphy

#include <fmt/format.h>

#endif // BAPHY_BAPHY_HPP
