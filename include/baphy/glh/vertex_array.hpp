#ifndef BAPHY_GLH_VERTEX_ARRAY_HPP
#define BAPHY_GLH_VERTEX_ARRAY_HPP

#include <initializer_list>
#include "baphy/gl.hpp"

namespace baphy::glh {
struct AttribBinding {
  const GLchar *name;
  GLint size;
  GLenum type;
  bool normalized;
  GLuint offset;
};

GLuint create_vertex_array_from_bindings(
    GLuint program_id, std::initializer_list<AttribBinding> args);
} // namespace baphy::glh

#endif // BAPHY_GLH_VERTEX_ARRAY_HPP
