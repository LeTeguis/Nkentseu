#version 450 core

layout (location=0) in vec4 ourColor;
layout (location=1) in vec2 TexCoord;

layout (location=0) out vec4 color;

layout(set=0, binding=1) uniform sampler2D textureSampler;

void main()
{
	//color = vec4(0.15, 0.15, 0.15, 0.15);
	color = ourColor;
	color = texture(textureSampler, TexCoord) * color;
}	