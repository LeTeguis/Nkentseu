#version 450 core
#extension GL_ARB_separate_shader_objects : enable

#ifdef VULKAN
#define PUSH_CONSTANT layout (push_constant)
#else
#define PUSH_CONSTANT layout (std140)
#endif

layout(location = 0) in vec2 aPos;   // Position attribute
layout(location = 1) in vec4 aColor; // Color attribute
layout(location = 2) in vec2 aTexCoord; // Texture coordinate attribute

layout (location=0) out vec4 shapeColor; // Output color
layout (location=1) out vec2 fragTexCoord; // Output texture coordinate

layout(set=0, binding=0) uniform CanvasCamera {
    mat4 view;
    mat4 proj;
} camera;

PUSH_CONSTANT uniform CanvasTransform {
    mat4 model;
} transform;

void main() {
    gl_Position = camera.proj * camera.view * transform.model * vec4(aPos, 0.0, 1.0);
    //gl_Position = vec4(aPos, 0.0, 1.0);
    shapeColor = aColor;
    fragTexCoord = aTexCoord;
}