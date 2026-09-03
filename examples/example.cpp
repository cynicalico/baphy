#include <fmt/format.h>
#include "baphy/baphy.hpp"

int main(int, char *[]) {
  fmt::println("baphy v{}", baphy::version());

  auto &r = baphy::Runner::instance();
  while (!r.window->should_close()) {
    r.poll_events();
    r.window->swap_buffers();
  }

  return 0;
}
