#include "baphy/glh/vertex_array.hpp"

#include <utility>

baphy::glh::VertexArray::VertexArray(GLuint program_id, std::span<const AttribBinding> attrib_bindings) {
    glCreateVertexArrays(1, &id);

    for (const auto &[name, size, type, normalized, offset]: attrib_bindings) {
        const auto location = glGetAttribLocation(program_id, name);

        glEnableVertexArrayAttrib(id, location);
        glVertexArrayAttribFormat(id, location, size, type, normalized ? GL_TRUE : GL_FALSE, offset);
        glVertexArrayAttribBinding(id, location, 0);
    }
}

baphy::glh::VertexArray::~VertexArray() {
    if (id != 0)
        glDeleteVertexArrays(1, &id);
}

baphy::glh::VertexArray::VertexArray(VertexArray &&other) noexcept
    : id(std::exchange(other.id, 0)) {}

baphy::glh::VertexArray &baphy::glh::VertexArray::operator=(VertexArray &&other) noexcept {
    if (this != &other) {
        if (id != 0)
            glDeleteVertexArrays(1, &id);

        id = std::exchange(other.id, 0);
    }
    return *this;
}
