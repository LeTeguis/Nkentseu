#version 420 core

layout (location=0) in vec3 fragmentColor;
layout (location=0) out vec4 screenColor;

void main(){
    screenColor = vec4(fragmentColor, 1.0);
}