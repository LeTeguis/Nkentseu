#version 450 core
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_debug_printf : enable

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

layout (location=0) out vec3 ourColor;

layout(binding = 0) uniform ObjectBuffer {
    mat4 model;
} objectBuffer;

layout(binding = 1) uniform CameraBuffer {
    mat4 view;
    mat4 proj;
} cameraBuffer;

void main()
{
    gl_Position = cameraBuffer.proj * cameraBuffer.view * objectBuffer.model * vec4(position, 1.0);
    //gl_Position = vec4(position, 1.0);
    ourColor = color;
}

/*


#ifdef VULKAN
layout (push_constant)
#endif
uniform Push {
  mat4 model;
} push;
*/