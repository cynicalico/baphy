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

void baphy::InputMgr::prev_propagate_() {
    mouse.delta = glm::vec2{0, 0};
    mouse.prev_pos = mouse.pos;

    for (const auto &button: mouse.buttons_state | std::views::keys)
        mouse.buttons_prev_state[button] = mouse.buttons_state[button];

    for (const auto &key: keyboard.keys_state | std::views::keys)
        keyboard.keys_prev_state[key] = keyboard.keys_state[key];

    for (const auto &scancode: keyboard.scancodes_state | std::views::keys)
        keyboard.scancodes_prev_state[scancode] = keyboard.scancodes_state[scancode];
}

bool baphy::InputMgr::button_pressed(const Button button) const {
    return mouse.buttons_state.contains(button) && mouse.buttons_state.at(button) &&
           (!mouse.buttons_prev_state.contains(button) || !mouse.buttons_prev_state.at(button));
}

bool baphy::InputMgr::button_released(const Button button) const {
    return mouse.buttons_state.contains(button) && !mouse.buttons_state.at(button) &&
           mouse.buttons_prev_state.contains(button) && mouse.buttons_prev_state.at(button);
}

bool baphy::InputMgr::button_down(const Button button) const {
    return mouse.buttons_state.contains(button) && mouse.buttons_state.at(button);
}

bool baphy::InputMgr::key_pressed(const Key key) const {
    return keyboard.keys_state.contains(key) && keyboard.keys_state.at(key) &&
           (!keyboard.keys_prev_state.contains(key) || !keyboard.keys_prev_state.at(key));
}

bool baphy::InputMgr::key_released(const Key key) const {
    return keyboard.keys_state.contains(key) && !keyboard.keys_state.at(key) &&
           keyboard.keys_prev_state.contains(key) && keyboard.keys_prev_state.at(key);
}

bool baphy::InputMgr::key_down(const Key key) const {
    return keyboard.keys_state.contains(key) && keyboard.keys_state.at(key);
}

bool baphy::InputMgr::scancode_pressed(const Scancode scancode) const {
    return keyboard.scancodes_state.contains(scancode) && keyboard.scancodes_state.at(scancode) &&
           (!keyboard.scancodes_prev_state.contains(scancode) || !keyboard.scancodes_prev_state.at(scancode));
}

bool baphy::InputMgr::scancode_released(const Scancode scancode) const {
    return keyboard.scancodes_state.contains(scancode) && !keyboard.scancodes_state.at(scancode) &&
           keyboard.scancodes_prev_state.contains(scancode) && keyboard.scancodes_prev_state.at(scancode);
}

bool baphy::InputMgr::scancode_down(const Scancode scancode) const {
    return keyboard.scancodes_state.contains(scancode) && keyboard.scancodes_state.at(scancode);
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
    switch (e.action) {
    case Action::Down:
        keyboard.keys_state[e.key] = true;
        keyboard.scancodes_state[e.scancode] = true;
        break;
    case Action::Up:
        keyboard.keys_state[e.key] = false;
        keyboard.scancodes_state[e.scancode] = false;
        break;
    case Action::Repeat:
        // do nothing
        break;
    }
}

void baphy::InputMgr::mouse_button_callback_(const MouseButtonEvent &e) {
    switch (e.action) {
    case Action::Down:
        mouse.buttons_state[e.button] = true;
        break;
    case Action::Up:
        mouse.buttons_state[e.button] = false;
        break;
    case Action::Repeat:
        std::unreachable();
    }
}

void baphy::InputMgr::mouse_motion_callback_(const MouseMotionEvent &e) {
    mouse.pos = glm::vec2{e.x, e.y};
    mouse.delta += glm::vec2{e.dx, e.dy};
}

void baphy::InputMgr::mouse_wheel_callback_(const MouseWheelEvent &e) {
    mouse.wheel += glm::vec2{e.x, e.y};
}
