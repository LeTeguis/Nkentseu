#version 450 core

layout(location=0) out vec4 FragColor;

//layout(binding = 2) uniform sampler2D texSampler;

layout (location=0) in vec2 fragTexCoord;

void main()
{
    FragColor = vec4(1.0, 0.0, 1.0, 1.0) ;//* texture(texSampler, fragTexCoord);;
}