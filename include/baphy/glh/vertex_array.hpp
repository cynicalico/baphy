#ifndef BAPHY_GLH_VERTEX_ARRAY_HPP
#define BAPHY_GLH_VERTEX_ARRAY_HPP

#include <span>
#include "baphy/gl.hpp"

namespace baphy::glh {
struct AttribBinding {
    const GLchar *name;
    GLint size;
    GLenum type;
    bool normalized;
    GLuint offset;
};

class VertexArray {
public:
    GLuint id{0};

    VertexArray(GLuint program_id, std::span<const AttribBinding> attrib_bindings);
    ~VertexArray();

    VertexArray(const VertexArray &other) = delete;
    VertexArray &operator=(const VertexArray &other) = delete;

    VertexArray(VertexArray &&other) noexcept;
    VertexArray &operator=(VertexArray &&other) noexcept;
};
} // namespace baphy::glh

#endif // BAPHY_GLH_VERTEX_ARRAY_HPP
