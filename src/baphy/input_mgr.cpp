#include "baphy/input_mgr.hpp"
#include "baphy/runner.hpp"

baphy::InputMgr::InputMgr(Runner &runner)
    : runner_(runner) {
  register_callbacks_();
}

baphy::InputMgr::~InputMgr() {
  if (callback_id_) {
    runner_.nexus->release_id(*callback_id_);
    callback_id_ = std::nullopt;
  }
}

baphy::InputMgr::InputMgr(InputMgr &&other) noexcept
    : runner_(other.runner_) {
  if (other.callback_id_) {
    other.runner_.nexus->release_id(*other.callback_id_);
    other.callback_id_ = std::nullopt;
  }

  register_callbacks_();
}

void baphy::InputMgr::update_(double dt) {
  mouse.dx = 0;
  mouse.dy = 0;
  mouse.px = mouse.x;
  mouse.py = mouse.y;
}

void baphy::InputMgr::register_callbacks_() {
  if (callback_id_)
    runner_.nexus->release_id(*callback_id_);

  callback_id_ = runner_.nexus->acquire_id();

  runner_.nexus->subscribe<KeyboardEvent>(*callback_id_, [&](const auto *e) {
    keyboard_callback_(*e);
  });

  runner_.nexus->subscribe<MouseButtonEvent>(*callback_id_, [&](const auto *e) {
    mouse_button_callback_(*e);
  });

  runner_.nexus->subscribe<MouseMotionEvent>(*callback_id_, [&](const auto *e) {
    mouse_motion_callback_(*e);
  });

  runner_.nexus->subscribe<MouseWheelEvent>(*callback_id_, [&](const auto *e) {
    mouse_wheel_callback_(*e);
  });
}

void baphy::InputMgr::keyboard_callback_(const KeyboardEvent &e) {
  // TODO
}

void baphy::InputMgr::mouse_button_callback_(const MouseButtonEvent &e) {
  // TODO
}

void baphy::InputMgr::mouse_motion_callback_(const MouseMotionEvent &e) {
  mouse.x = e.x;
  mouse.y = e.y;
  mouse.dx += e.dx;
  mouse.dy += e.dy;
}

void baphy::InputMgr::mouse_wheel_callback_(const MouseWheelEvent &e) {
  mouse.wheel.x += e.x;
  mouse.wheel.y += e.y;
}
