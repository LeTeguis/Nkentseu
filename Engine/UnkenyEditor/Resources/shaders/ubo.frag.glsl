#version 450 core

layout (location=0) in vec3 ourColor;
layout (location=1) in vec2 fragTexCoord;

layout (location=0) out vec4 color;

//uniform vec4 u_color;
layout(binding = 1) uniform sampler2D texSampler;

void main()
{
	//color = vec4(0.15, 0.15, 0.15, 0.15);
	//color = vec4(ourColor, 1.0);
	color = texture(texSampler, fragTexCoord);
}	