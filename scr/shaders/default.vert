#version 430

layout(location = 0)in vec4 vert;

void main() {
    gl_Position = vert;
}