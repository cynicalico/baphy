#include "baphy/glh/vertex_array.hpp"

GLuint baphy::glh::create_vertex_array_from_bindings(
    const GLuint program_id, std::initializer_list<AttribBinding> args) {
  GLuint id;
  glCreateVertexArrays(1, &id);

  for (const auto &[name, size, type, normalized, offset]: args) {
    const auto location = glGetAttribLocation(program_id, name);

    glEnableVertexArrayAttrib(id, location);
    glVertexArrayAttribFormat(
        id, location, size, type, normalized ? GL_TRUE : GL_FALSE, offset);
    glVertexArrayAttribBinding(id, location, 0);
  }

  return id;
}
