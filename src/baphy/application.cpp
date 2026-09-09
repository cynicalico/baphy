#include "baphy/application.hpp"
#include "baphy/event/all.hpp"
#include "baphy/runner.hpp"

baphy::Application::Application(Runner &runner)
    : runner{runner} {
  subscribe_callbacks_();
}

baphy::Application::~Application() {
  runner.nexus->release_id(callback_id_);
}

void baphy::Application::update(double) {}

void baphy::Application::draw() {}

void baphy::Application::char_callback(unsigned int) {}

void baphy::Application::cursor_enter_callback(bool) {}

void baphy::Application::cursor_pos_callback(double, double) {}

void baphy::Application::drop_callback(const std::vector<std::string> &) {}

void baphy::Application::mouse_button_callback(Button, Action, ModFlags) {}

void baphy::Application::key_callback(Key, Action, ModFlags) {}

void baphy::Application::scroll_callback(double, double) {}

void baphy::Application::subscribe_callbacks_() {
  callback_id_ = runner.nexus->acquire_id();

  runner.nexus->subscribe<CharEvent>(callback_id_, [&](const CharEvent *p) {
    char_callback(p->codepoint);
  });

  runner.nexus->subscribe<CursorEnterEvent>(
      callback_id_, [&](const CursorEnterEvent *p) {
        cursor_enter_callback(p->entered);
      });

  runner.nexus->subscribe<CursorPosEvent>(
      callback_id_, [&](const CursorPosEvent *p) {
        cursor_pos_callback(p->x, p->y);
      });

  runner.nexus->subscribe<DropEvent>(callback_id_, [&](const DropEvent *p) {
    drop_callback(p->paths);
  });

  runner.nexus->subscribe<MouseButtonEvent>(
      callback_id_, [&](const MouseButtonEvent *p) {
        mouse_button_callback(p->button, p->action, p->mods);
      });

  runner.nexus->subscribe<KeyEvent>(callback_id_, [&](const KeyEvent *p) {
    key_callback(p->key, p->action, p->mods);
  });

  runner.nexus->subscribe<ScrollEvent>(callback_id_, [&](const ScrollEvent *p) {
    scroll_callback(p->dx, p->dy);
  });
}
