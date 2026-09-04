#include "baphy/baphy.hpp"

static void run() {
  BAPHY_LOG_DEBUG("baphy v{}", baphy::version());

  auto &r = baphy::Runner::instance();
  while (!r.window->should_close()) {
    r.poll_events();

    glClearColor(0.2f, 0.1f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    r.window->swap_buffers();
  }
}

int main(int, char *[]) {
  try {
    run();
  } catch (std::runtime_error &e) {
    BAPHY_LOG_ERROR("Runtime error: {}", e.what());
    return 1;
  }

  return 0;
}
