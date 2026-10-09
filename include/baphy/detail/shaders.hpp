#ifndef BAPHY_DETAIL_SHADERS_HPP
#define BAPHY_DETAIL_SHADERS_HPP

namespace baphy::detail {
constexpr auto PRIMITIVE_VERT_SRC = R"glsl(
#version 460 core

in vec3 aPos;
in vec4 aColor;

out vec4 frag_color;

uniform mat4 projection;
uniform float z_max;

void main() {
    frag_color = aColor;

    gl_Position = projection * vec4(aPos.x, aPos.y, -aPos.z / z_max, 1.0);
}
)glsl";

constexpr auto PRIMITIVE_FRAG_SRC = R"glsl(
#version 460 core

out vec4 FragColor;

in vec4 frag_color;

void main() {
    FragColor = frag_color;
}
)glsl";

constexpr auto TEXTURE_VERT_SRC = R"glsl(
#version 460 core

in vec3 aPos;
in vec4 aColor;
in vec2 aTexCoord;

out vec4 frag_color;
out vec2 frag_tex_coord;

uniform mat4 projection;
uniform float z_max;

void main() {
    frag_color = aColor;
    frag_tex_coord = aTexCoord;

    gl_Position = projection * vec4(aPos.x, aPos.y, -aPos.z / z_max, 1.0);
}
)glsl";

constexpr auto TEXTURE_FRAG_SRC = R"glsl(
#version 460 core

out vec4 FragColor;

in vec4 frag_color;
in vec2 frag_tex_coord;

uniform sampler2D tex;

void main() {
    FragColor = frag_color * texture(tex, frag_tex_coord);
}
)glsl";
} // namespace baphy::detail

#endif // BAPHY_DETAIL_SHADERS_HPP
