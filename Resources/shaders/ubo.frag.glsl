#version 450 core

layout (location=0) in vec4 ourColor;
layout (location=1) in vec2 fragTexCoord;

layout (location=0) out vec4 color;

//uniform vec4 u_color;
layout(binding = 2) uniform sampler2D textureSampler;

void main()
{
	//color = vec4(0.15, 0.15, 0.15, 0.15);
	//color = vec4(ourColor.x, ourColor.y, ourColor.z, 1.0);
	//color = ourColor;
	color = texture(textureSampler, fragTexCoord);
}	