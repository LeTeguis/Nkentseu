#version 450 core

#ifdef VULKAN
#define PUSH_CONSTANT layout (push_constant)
#define UNIFORM_BINDING(set_data, binding_data) set=set_data, binding=binding_data
#else
#define PUSH_CONSTANT layout (std140)
#define UNIFORM_BINDING(set_data, binding_data) binding=binding_data
#endif

const uint USE_COLOR = 1 << 1;
const uint USE_TEXTURE = 1 << 2;
const uint USE_TEXT = 1 << 3;

#define TEST_USE_INFO(data, info) ((data & (info)) == (info))

layout (location=0) in vec4 shapeColor;
layout (location=1) in vec2 fragTexCoord;

layout (location=0) out vec4 mixColor;

PUSH_CONSTANT uniform Material2D {
    uint useFlags;
} material2d;

layout(UNIFORM_BINDING(1, 1)) uniform sampler2D s2dTexture;
layout(UNIFORM_BINDING(2, 2)) uniform sampler2D s2dText;

void main()
{   
    vec4 finalColor = vec4(1.0, 1.0, 1.0, 1.0);

    if (TEST_USE_INFO(material2d.useFlags, USE_TEXT)) {
        finalColor = vec4(1.0, 1.0, 1.0, texture(s2dText, fragTexCoord).r);
    }

    if (TEST_USE_INFO(material2d.useFlags, USE_COLOR)) {
        finalColor *= shapeColor;
    }

    if (TEST_USE_INFO(material2d.useFlags, USE_TEXTURE)) {
        finalColor *= texture(s2dTexture, fragTexCoord);
    }

    if (material2d.useFlags == 0) finalColor = vec4(1.0, 0.0, 1.0, 1.0);

    mixColor = finalColor;
}