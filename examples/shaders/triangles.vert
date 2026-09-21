#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 frag_color;

void main() {
  frag_color = aColor;

  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
