#version 450 core

layout (location=0) in vec4 shapeColor;
layout (location=1) in vec2 fragTexCoord;

layout (location=0) out vec4 mixColor;

layout(binding = 2) uniform CanvasMaterial {
    int useTexture;
    int useColor;
} material;

//layout(binding = 3) uniform sampler2D texSampler;

void main()
{
    vec4 finalColor = vec4(1.0, 1.0, 1.0, 1.0);
    //vec4 finalColor = shapeColor;

    if (material.useColor > 0){
        finalColor *= shapeColor;
    }

    if (material.useTexture > 0){
        //finalColor *= texture(texSampler, fragTexCoord);
    }

    mixColor = finalColor;
}