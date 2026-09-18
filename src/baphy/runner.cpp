#include "baphy/runner.hpp"

#include <SDL3/SDL.h>
#include <fmt/format.h>
#include <glad/gl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <stdexcept>
#include "baphy/event/all.hpp"
#include "baphy/log.hpp"

baphy::Runner::Runner() {
  nexus = std::make_unique<nexus::Nexus>();
}

baphy::Runner::~Runner() {
  if (imgui_ctx_) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext(imgui_ctx_);
    imgui_ctx_ = nullptr;
  }

  window.reset();
  SDL_Quit();

  nexus.reset();
}

void baphy::Runner::open_window_(const WindowOpts &opts) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(
        fmt::format("Failed to initialize SDL! {}", SDL_GetError()));
  }
  BAPHY_LOG_DEBUG("SDL v{}.{}.{}",
                  SDL_VERSIONNUM_MAJOR(SDL_VERSION),
                  SDL_VERSIONNUM_MINOR(SDL_VERSION),
                  SDL_VERSIONNUM_MICRO(SDL_VERSION));

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#ifndef NDEBUG
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif
  window = std::make_unique<Window>(opts);

  SDL_GL_MakeCurrent(window->handle(), window->context());
  if (gladLoadGL(SDL_GL_GetProcAddress) == 0)
    throw std::runtime_error("Failed to initialize GLAD!");
  BAPHY_LOG_DEBUG("OpenGL v{}",
                  reinterpret_cast<const char *>(glGetString(GL_VERSION)));
  BAPHY_LOG_DEBUG("OpenGL renderer: {}",
                  reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
  BAPHY_LOG_DEBUG("OpenGL vendor: {}",
                  reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
}

void baphy::Runner::initialize_imgui_() {
  IMGUI_CHECKVERSION();
  imgui_ctx_ = ImGui::CreateContext();
  if (!imgui_ctx_)
    throw std::runtime_error("Failed to initialize ImGui!");

  ImGui::StyleColorsDark();
  ImGui::GetIO().IniFilename = nullptr;

  if (!ImGui_ImplSDL3_InitForOpenGL(window->handle(), window->context()))
    throw std::runtime_error("Failed to initialize ImGui SDL3 backend!");
  if (!ImGui_ImplOpenGL3_Init(nullptr))
    throw std::runtime_error("Failed to initialize ImGui OpenGL3 backend!");
  BAPHY_LOG_DEBUG("ImGui v{}", ImGui::GetVersion());
}

void baphy::Runner::run_() {
  if (!SDL_GL_SetSwapInterval(1))
    BAPHY_LOG_WARN("Failed to enable vsync: {}", SDL_GetError());

  while (!window->should_close()) {
    poll_events_();
    app_->update(0.0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    app_->draw();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    window->swap_buffers();
  }
}

void baphy::Runner::poll_events_() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL3_ProcessEvent(&event);

    switch (event.type) {
    case SDL_EVENT_QUIT:
      nexus->publish<QuitEvent>(event.quit.timestamp);
      break;

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
      nexus->publish<KeyboardEvent>(
          event.key.timestamp,
          action_from_sdl_keyboard_event(&event.key),
          static_cast<Scancode>(event.key.scancode),
          static_cast<Key>(event.key.key),
          static_cast<ModFlags>(event.key.mod),
          event.key.raw);
      break;

    case SDL_EVENT_TEXT_EDITING:
    case SDL_EVENT_TEXT_INPUT:
    case SDL_EVENT_KEYMAP_CHANGED:
      break; // TODO

    case SDL_EVENT_KEYBOARD_ADDED:
    case SDL_EVENT_KEYBOARD_REMOVED:
    case SDL_EVENT_TEXT_EDITING_CANDIDATES:
    case SDL_EVENT_SCREEN_KEYBOARD_SHOWN:
    case SDL_EVENT_SCREEN_KEYBOARD_HIDDEN:
      break; // TODO

    case SDL_EVENT_MOUSE_MOTION:
      nexus->publish<MouseMotionEvent>(
          event.motion.timestamp,
          static_cast<ButtonFlags>(event.motion.state),
          event.motion.x,
          event.motion.y,
          event.motion.xrel,
          event.motion.yrel);
      break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
      nexus->publish<MouseButtonEvent>(
          event.button.timestamp,
          action_from_sdl_mouse_button_event(&event.button),
          static_cast<Button>(event.button.button),
          event.button.clicks,
          event.button.x,
          event.button.y);
      break;

    case SDL_EVENT_MOUSE_WHEEL:
      nexus->publish<MouseWheelEvent>(
          event.wheel.timestamp,
          event.wheel.x,
          event.wheel.y,
          static_cast<WheelDirection>(event.wheel.direction),
          event.wheel.mouse_x,
          event.wheel.mouse_y,
          event.wheel.integer_x,
          event.wheel.integer_y);
      break;

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
      BAPHY_LOG_WARN("Unhandled SDL event: {:08x}", event.type);
    }
  }
}
