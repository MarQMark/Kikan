#version 430

layout(location = 0) in float textureSlot;
layout(location = 1) in vec4 position;
layout(location = 2) in vec2 texCoords;
layout(location = 3) in vec4 color;

uniform mat4 u_mvp;

out vec4 v_color;

void main() {
    v_color = color;
    gl_Position = u_mvp * position;
}