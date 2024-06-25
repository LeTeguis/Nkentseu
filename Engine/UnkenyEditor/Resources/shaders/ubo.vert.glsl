#version 450 core
#extension GL_ARB_separate_shader_objects : enable
//#extension GL_EXT_debug_printf : enable

layout (location=0) in vec3 position;
layout (location=1) in vec4 color;
layout (location=2) in vec2 uv;

layout (location=0) out vec4 ourColor;
layout (location=1) out vec2 fragTexCoord;

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
    fragTexCoord = uv;
}

/*


#ifdef VULKAN
layout (push_constant)
#endif
uniform Push {
  mat4 model;
} push;
*/