#version 330 core

in vec3 fragColor;

out vec4 colorOut;

void main() {
    colorOut = vec4(fragColor, 1.0f);
} 