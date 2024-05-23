#version 420 core

layout (location=0) in vec3 vertexPos;
// layout (location=1) in vec3 vertexColor;

layout (location=0) out vec3 fragmentColor;

void main() {
    gl_Position = vec4(vertexPos, 1.0);
    // fragmentColor = vertexColor;
    fragmentColor = vec3(0.0 / 255.0, 162 / 255.0, 232 / 255.0);
}