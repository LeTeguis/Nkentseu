#version 450 core

layout (location=0) in vec3 position;
layout (location=1) in vec4 color;
layout (location=2) in vec2 uv;

layout (location=0) out vec2 fragTexCoord;

const vec2 positions[3] = vec2[](
    vec2(0.0,  0.5),
    vec2(0.5, -0.5),
    vec2(-0.5, -0.5)
);

const vec2 fragTexCoords[3] = vec2[](
    vec2(0.0,  0.0),
    vec2(1.0, 0.0),
    vec2(0.5, 1.0)
);

layout(binding = 0) uniform ObjectBuffer {
    mat4 model;
} objectBuffer;

layout(binding = 1) uniform CameraBuffer {
    mat4 view;
    mat4 proj;
} cameraBuffer;

void main()
{
    //gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    //gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    //gl_Position = cameraBuffer.proj * cameraBuffer.view * objectBuffer.model * vec4(positions[gl_VertexID], 0.0, 1.0);
    gl_Position = cameraBuffer.proj * cameraBuffer.view * objectBuffer.model * vec4(position, 1.0);
    fragTexCoord = fragTexCoords[gl_VertexID];
}