
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

#ifdef VULKAN
#define PUSH_CONSTANT layout (push_constant)
#else
#define PUSH_CONSTANT layout (std140)
#endif

layout (location=0) in vec3 position;
layout (location=1) in vec4 color;
layout (location=2) in vec2 uv;

layout (location=0) out vec4 ourColor;
layout (location=1) out vec2 fragTexCoord;

layout(set=0, binding=0) uniform CameraBuffer {
    mat4 view;
    mat4 proj;
} cameraBuffer;

PUSH_CONSTANT uniform ObjectBuffer {
    mat4 model;
} objectBuffer;

void main()
{
    gl_Position = cameraBuffer.proj * cameraBuffer.view * objectBuffer.model * vec4(position, 1.0);
    //gl_Position = vec4(position, 1.0) * objectBuffer.model * cameraBuffer.view * cameraBuffer.proj;
    //gl_Position = vec4(position, 1.0);
    ourColor = color;
    fragTexCoord = uv;
}