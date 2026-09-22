#include "baphy/gl.hpp"
#include <utility>
#include "baphy/log.hpp"

#ifndef NDEBUG
static constexpr const char *gl_debug_source_to_str(GLenum source);
static constexpr const char *gl_debug_type_to_str(GLenum type);

void baphy::gl_debug_message_callback(
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
