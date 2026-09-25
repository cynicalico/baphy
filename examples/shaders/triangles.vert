#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 frag_color;

uniform mat4 projection;

void main() {
    frag_color = aColor;

    gl_Position = projection * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
