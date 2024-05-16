#version 450 core

in vec3 fragmentColor;
out vec3 screenColor;

void main(){
    screenColor = vec4(fragmentColor, 1.0);
}