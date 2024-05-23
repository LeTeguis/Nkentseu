#version 450 core

layout (location=0) in vec3 ourColor;

layout (location=0) out vec4 color;

//uniform vec4 u_color;

void main()
{
	color = vec4(0.15, 0.15, 0.15, 0.15);
}	