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

baphy::Color baphy::Application::clear_color() {
  return rgb(0, 0, 0);
}

void baphy::Application::update(double) {}

void baphy::Application::draw() {}

void baphy::Application::keyboard_callback(const KeyboardEvent &) {}
void baphy::Application::mouse_button_callback(const MouseButtonEvent &) {}
void baphy::Application::mouse_motion_callback(const MouseMotionEvent &) {}
void baphy::Application::mouse_wheel_callback(const MouseWheelEvent &) {}
void baphy::Application::quit_callback(const QuitEvent &) {}

void baphy::Application::subscribe_callbacks_() {
  if (callback_id_ != 0)
    runner.nexus->release_id(callback_id_);

  callback_id_ = runner.nexus->acquire_id();

  runner.nexus->subscribe<KeyboardEvent>(callback_id_, [&](const auto *e) {
    keyboard_callback(*e);
  });

  runner.nexus->subscribe<MouseButtonEvent>(callback_id_, [&](const auto *e) {
    mouse_button_callback(*e);
  });

  runner.nexus->subscribe<MouseMotionEvent>(callback_id_, [&](const auto *e) {
    mouse_motion_callback(*e);
  });

  runner.nexus->subscribe<MouseWheelEvent>(callback_id_, [&](const auto *e) {
    mouse_wheel_callback(*e);
  });

  runner.nexus->subscribe<QuitEvent>(callback_id_, [&](const auto *e) {
    runner.window->set_should_close(true);

    quit_callback(*e);
  });
}
