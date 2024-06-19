#version 450 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;
layout (location=2) in vec2 uv;

layout (location=0) out vec3 ourColor;
layout (location=1) out vec2 TexCoord;

void main()
{
    gl_Position = vec4(position, 1.0f);
    ourColor = color;
    TexCoord = uv;
    //ourColor = vec3(0.15, 0.15, 0.15);
}