#include "baphy/runner.hpp"

#include <SDL3/SDL.h>
#include <fmt/format.h>
#include <glad/gl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <stdexcept>
#include "baphy/event/all.hpp"
#include "baphy/log.hpp"

#ifndef NDEBUG
static void gl_debug_message_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam);
#endif

baphy::Runner::Runner() {
  nexus = std::make_unique<nexus::Nexus>();
}

baphy::Runner::~Runner() {
  app_.reset();

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

#ifndef NDEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(gl_debug_message_callback, nullptr);
#endif
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

  implot_ctx_ = ImPlot::CreateContext();
  if (!implot_ctx_)
    throw std::runtime_error("Failed to initialize ImPlot!");
}

void baphy::Runner::run_() {
  if (!window->set_swap_interval(SwapInterval::Adaptive)) {
    if (!window->set_swap_interval(SwapInterval::VSync))
      BAPHY_LOG_WARN("Failed to enable vsync: {}", SDL_GetError());
  }

  frame_counter.reset();

  while (!window->should_close()) {
    poll_events_();
    app_->update(as_secs_dt(frame_counter.dt()));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    glViewport(0, 0, window->pixel_w(), window->pixel_h());

    const auto clear_color = app_->clear_color().value();
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    app_->draw();

    draw_fps_overlay_();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    window->swap_buffers();

    frame_counter.update();
    first_frame = false;
  }
}

void baphy::Runner::poll_events_() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    ImGui_ImplSDL3_ProcessEvent(&event);

    bool imgui_handled;
    switch (event.type) {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
    case SDL_EVENT_TEXT_EDITING:
    case SDL_EVENT_TEXT_INPUT:
    case SDL_EVENT_TEXT_EDITING_CANDIDATES:
      imgui_handled = ImGui::GetIO().WantCaptureKeyboard;
      break;

    case SDL_EVENT_MOUSE_MOTION:
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
    case SDL_EVENT_MOUSE_WHEEL:
      imgui_handled = ImGui::GetIO().WantCaptureMouse;
      break;

    default:
      imgui_handled = false;
      break;
    }
    if (imgui_handled)
      continue;

    if (const auto baphy_event = event_from_sdl_event(event); baphy_event) {
      std::visit(
          [&](const auto &e) {
            nexus->publish_ptr(&e);
          },
          *baphy_event);
    }
  }
}

void baphy::Runner::draw_fps_overlay_() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  if (ImGui::Begin(
          "fps_overlay",
          nullptr,
          ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration)) {
    ImGui::Text("FPS: %.2f", frame_counter.fps());
  }
  ImGui::End();

  ImGui::PopStyleVar();
}

#ifndef NDEBUG
static constexpr const char *gl_debug_source_to_str(GLenum source);
static constexpr const char *gl_debug_type_to_str(GLenum type);

void gl_debug_message_callback(
    const GLenum source,
    const GLenum type,
    const GLuint id,
    const GLenum severity,
    const GLsizei,
    const GLchar *message,
    const void *) {
  const char *source_str = gl_debug_source_to_str(source);
  const char *type_str = gl_debug_type_to_str(type);

  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    BAPHY_LOG_ERROR(
        "OpenGL [{}] [{}] id={}: {}", source_str, type_str, id, message);
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    BAPHY_LOG_WARN(
        "OpenGL [{}] [{}] id={}: {}", source_str, type_str, id, message);
    break;
  case GL_DEBUG_SEVERITY_LOW:
    BAPHY_LOG_DEBUG(
        "OpenGL [{}] [{}] id={}: {}", source_str, type_str, id, message);
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    BAPHY_LOG_TRACE(
        "OpenGL [{}] [{}] id={}: {}", source_str, type_str, id, message);
    break;
  default:
    std::unreachable();
  }
}

constexpr const char *gl_debug_source_to_str(const GLenum source) {
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    return "API";
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    return "WINDOW_SYSTEM";
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    return "SHADER_COMPILER";
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    return "THIRD_PARTY";
  case GL_DEBUG_SOURCE_APPLICATION:
    return "APPLICATION";
  case GL_DEBUG_SOURCE_OTHER:
    return "OTHER";
  default:
    std::unreachable();
  }
}

constexpr const char *gl_debug_type_to_str(const GLenum type) {
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    return "ERROR";
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    return "DEPRECATED_BEHAVIOR";
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    return "UNDEFINED_BEHAVIOR";
  case GL_DEBUG_TYPE_PORTABILITY:
    return "PORTABILITY";
  case GL_DEBUG_TYPE_PERFORMANCE:
    return "PERFORMANCE";
  case GL_DEBUG_TYPE_MARKER:
    return "MARKER";
  case GL_DEBUG_TYPE_PUSH_GROUP:
    return "PUSH_GROUP";
  case GL_DEBUG_TYPE_POP_GROUP:
    return "POP_GROUP";
  case GL_DEBUG_TYPE_OTHER:
    return "OTHER";
  default:
    std::unreachable();
  }
}
#endif
