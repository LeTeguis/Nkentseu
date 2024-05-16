#version 450 core

layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexColor;

out vec3 fragmentColor;

void main(){
    gl_position = vec4(vertexPos, 1.0);
    fragmentColor = vertexColor;
}