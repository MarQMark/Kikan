#version 430

layout(location = 0) in float textureSlot;
layout(location = 1) in vec3 position;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec4 color;

out vec4 v_color;

void main() {
    v_color = color;
    gl_Position = vec4(position, 1.0);
}