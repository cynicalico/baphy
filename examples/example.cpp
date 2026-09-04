#include "baphy/baphy.hpp"
#include "baphy/event/key.hpp"

static void run() {
  BAPHY_LOG_DEBUG("baphy v{}", baphy::version());

  auto &r = baphy::Runner::instance();
  auto &w = r.window;

  const auto id = r.nexus->acquire_id();

  r.nexus->subscribe<baphy::KeyEvent>(id, [&w](const auto *p) {
    if (p->key == baphy::Key::Escape && p->action == baphy::Action::Press)
      w->set_should_close(true);
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
