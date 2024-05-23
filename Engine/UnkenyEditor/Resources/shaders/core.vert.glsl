#version 420 core
layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;

layout (location=0) out vec3 ourColor;

void main()
{
    gl_Position = vec4(position, 1.0f);
    // ourColor = color;
    ourColor = vec3(0.15, 0.15, 0.15);
}