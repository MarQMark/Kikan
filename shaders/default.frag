#version 430 core

in vec4 v_color;

layout(location = 0) out vec4 fragColor;

void main() {
    fragColor = v_color;
}