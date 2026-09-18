#ifndef BAPHY_EVENT_ALL_HPP
#define BAPHY_EVENT_ALL_HPP

#include <optional>
#include <variant>
#include "baphy/event/common.hpp"
#include "baphy/event/keyboard.hpp"
#include "baphy/event/mouse.hpp"
#include "baphy/event/quit.hpp"

namespace baphy {
using Event = std::variant<KeyboardEvent,
                           MouseButtonEvent,
                           MouseMotionEvent,
                           MouseWheelEvent,
                           QuitEvent>;

constexpr std::optional<Event> event_from_sdl_event(const SDL_Event &e) {
  switch (e.type) {
  case SDL_EVENT_QUIT:
    return QuitEvent{e.quit.timestamp};

  case SDL_EVENT_DISPLAY_ORIENTATION:
  case SDL_EVENT_DISPLAY_ADDED:
  case SDL_EVENT_DISPLAY_REMOVED:
  case SDL_EVENT_DISPLAY_MOVED:
  case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
  case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
  case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
  case SDL_EVENT_DISPLAY_USABLE_BOUNDS_CHANGED:
    break; // TODO

  case SDL_EVENT_WINDOW_SHOWN:
  case SDL_EVENT_WINDOW_HIDDEN:
  case SDL_EVENT_WINDOW_EXPOSED:
  case SDL_EVENT_WINDOW_MOVED:
  case SDL_EVENT_WINDOW_RESIZED:
  case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
  case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
  case SDL_EVENT_WINDOW_MINIMIZED:
  case SDL_EVENT_WINDOW_MAXIMIZED:
  case SDL_EVENT_WINDOW_RESTORED:
  case SDL_EVENT_WINDOW_MOUSE_ENTER:
  case SDL_EVENT_WINDOW_MOUSE_LEAVE:
  case SDL_EVENT_WINDOW_FOCUS_GAINED:
  case SDL_EVENT_WINDOW_FOCUS_LOST:
  case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
  case SDL_EVENT_WINDOW_HIT_TEST:
  case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
  case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
  case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
  case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
  case SDL_EVENT_WINDOW_OCCLUDED:
  case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
  case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
  case SDL_EVENT_WINDOW_DESTROYED:
  case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
    break; // TODO

  case SDL_EVENT_KEY_DOWN:
  case SDL_EVENT_KEY_UP:
    return KeyboardEvent{
        e.key.timestamp,
        action_from_sdl_keyboard_event(&e.key),
        static_cast<Scancode>(e.key.scancode),
        static_cast<Key>(e.key.key),
        static_cast<ModFlags>(e.key.mod),
        e.key.raw};

  case SDL_EVENT_TEXT_EDITING:
  case SDL_EVENT_TEXT_INPUT:
  case SDL_EVENT_KEYMAP_CHANGED:
  case SDL_EVENT_KEYBOARD_ADDED:
  case SDL_EVENT_KEYBOARD_REMOVED:
  case SDL_EVENT_TEXT_EDITING_CANDIDATES:
  case SDL_EVENT_SCREEN_KEYBOARD_SHOWN:
  case SDL_EVENT_SCREEN_KEYBOARD_HIDDEN:
    break; // TODO

  case SDL_EVENT_MOUSE_MOTION:
    return MouseMotionEvent{
        e.motion.timestamp,
        static_cast<ButtonFlags>(e.motion.state),
        e.motion.x,
        e.motion.y,
        e.motion.xrel,
        e.motion.yrel};

  case SDL_EVENT_MOUSE_BUTTON_DOWN:
  case SDL_EVENT_MOUSE_BUTTON_UP:
    return MouseButtonEvent{
        e.button.timestamp,
        action_from_sdl_mouse_button_event(&e.button),
        static_cast<Button>(e.button.button),
        e.button.clicks,
        e.button.x,
        e.button.y};

  case SDL_EVENT_MOUSE_WHEEL:
    return MouseWheelEvent{
        e.wheel.timestamp,
        e.wheel.x,
        e.wheel.y,
        static_cast<WheelDirection>(e.wheel.direction),
        e.wheel.mouse_x,
        e.wheel.mouse_y,
        e.wheel.integer_x,
        e.wheel.integer_y};

  case SDL_EVENT_MOUSE_ADDED:
  case SDL_EVENT_MOUSE_REMOVED:
    break; // TODO

  case SDL_EVENT_JOYSTICK_AXIS_MOTION:
  case SDL_EVENT_JOYSTICK_BALL_MOTION:
  case SDL_EVENT_JOYSTICK_HAT_MOTION:
  case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
  case SDL_EVENT_JOYSTICK_BUTTON_UP:
  case SDL_EVENT_JOYSTICK_ADDED:
  case SDL_EVENT_JOYSTICK_REMOVED:
  case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
  case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
    break; // TODO

  case SDL_EVENT_GAMEPAD_AXIS_MOTION:
  case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
  case SDL_EVENT_GAMEPAD_BUTTON_UP:
  case SDL_EVENT_GAMEPAD_ADDED:
  case SDL_EVENT_GAMEPAD_REMOVED:
  case SDL_EVENT_GAMEPAD_REMAPPED:
  case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
  case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
  case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
  case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
  case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
  case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
    break; // TODO

  case SDL_EVENT_FINGER_DOWN:
  case SDL_EVENT_FINGER_UP:
  case SDL_EVENT_FINGER_MOTION:
  case SDL_EVENT_FINGER_CANCELED:
    break; // TODO

  case SDL_EVENT_PINCH_BEGIN:
  case SDL_EVENT_PINCH_UPDATE:
  case SDL_EVENT_PINCH_END:
    break; // TODO

  case SDL_EVENT_CLIPBOARD_UPDATE:
    break; // TODO

  case SDL_EVENT_DROP_FILE:
  case SDL_EVENT_DROP_TEXT:
  case SDL_EVENT_DROP_BEGIN:
  case SDL_EVENT_DROP_COMPLETE:
  case SDL_EVENT_DROP_POSITION:
    break; // TODO

  case SDL_EVENT_AUDIO_DEVICE_ADDED:
  case SDL_EVENT_AUDIO_DEVICE_REMOVED:
  case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
    break; // TODO

  case SDL_EVENT_SENSOR_UPDATE:
    break; // TODO

  case SDL_EVENT_PEN_PROXIMITY_IN:
  case SDL_EVENT_PEN_PROXIMITY_OUT:
  case SDL_EVENT_PEN_DOWN:
  case SDL_EVENT_PEN_UP:
  case SDL_EVENT_PEN_BUTTON_DOWN:
  case SDL_EVENT_PEN_BUTTON_UP:
  case SDL_EVENT_PEN_MOTION:
  case SDL_EVENT_PEN_AXIS:
    break; // TODO

  case SDL_EVENT_CAMERA_DEVICE_ADDED:
  case SDL_EVENT_CAMERA_DEVICE_REMOVED:
  case SDL_EVENT_CAMERA_DEVICE_APPROVED:
  case SDL_EVENT_CAMERA_DEVICE_DENIED:
    break; // TODO

  case SDL_EVENT_RENDER_TARGETS_RESET:
  case SDL_EVENT_RENDER_DEVICE_RESET:
  case SDL_EVENT_RENDER_DEVICE_LOST:
    break; // TODO

  default:
    break;
  }

  return std::nullopt;
}
} // namespace baphy

#endif // BAPHY_EVENT_ALL_HPP
