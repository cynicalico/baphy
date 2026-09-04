#include "baphy/baphy.hpp"
#include "baphy/event/keyboard.hpp"

static void run() {
  BAPHY_LOG_DEBUG("baphy v{}", baphy::version());

  auto &r = baphy::Runner::instance();
  auto &w = r.window;

  const auto id = r.nexus->acquire_id();
  r.nexus->subscribe<baphy::KeyEvent>(id, [](const auto *p) {
    BAPHY_LOG_INFO("Key event: key={}, scancode={}, action={}, mods={}",
                   p->key,
                   p->scancode,
                   p->action,
                   p->mods);
  });

  while (!w->should_close()) {
    r.poll_events();

    glClearColor(0.2f, 0.1f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    w->swap_buffers();
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
