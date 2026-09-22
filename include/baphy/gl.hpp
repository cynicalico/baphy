#ifndef BAPHY_GL_HPP
#define BAPHY_GL_HPP

#include <glad/gl.h>

namespace baphy {
#ifndef NDEBUG
void gl_debug_message_callback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar *message,
    const void *userParam);
#endif
} // namespace baphy

#endif // BAPHY_GL_HPP
