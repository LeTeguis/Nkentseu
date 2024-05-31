#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

layout (location=0) out vec3 ourColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

void main()
{
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(position, 1.0);
    ourColor = color;
    //ourColor = vec3(0.15, 0.15, 0.15);
}