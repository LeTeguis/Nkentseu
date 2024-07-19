#version 450 core

#ifdef VULKAN
#define PUSH_CONSTANT layout (push_constant)
#else
#define PUSH_CONSTANT layout (std140)
#endif

#define USE_COLOR (1 << 1)
#define USE_TEXTURE (1 << 2)

layout (location=0) in vec4 shapeColor;
layout (location=1) in vec2 fragTexCoord;

layout (location=0) out vec4 mixColor;

PUSH_CONSTANT uniform Material2D {
    int useTexture;
    int useColor;
} material2d;

layout(set=1, binding = 1) uniform sampler2D TextureSampler;

void main()
{
    vec4 finalColor = vec4(1.0, 1.0, 1.0, 1.0);

    if ((material2d.useColor & USE_COLOR) != 0){
        finalColor *= shapeColor;
    }

    if ((material2d.useTexture & USE_TEXTURE) != 0){
        finalColor *= texture(TextureSampler, fragTexCoord);
    }

    mixColor = finalColor;
}