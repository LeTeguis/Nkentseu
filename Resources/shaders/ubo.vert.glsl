#version 450 core
#extension GL_ARB_separate_shader_objects : enable
#extension GL_EXT_debug_printf : enable

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

layout (location=0) out vec3 ourColor;

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

// Fonction pour vérifier si une matrice est nulle
bool isMatrixNull(mat4 matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (matrix[i][j] != 0.0) {
                return false;
            }
        }
    }
    return true;
}

void main()
{
    mat4 model = mat4(1.0f);
    mat4 view = mat4(1.0f);
    mat4 proj = mat4(1.0f);

    if (!isMatrixNull(ubo.proj)) {
        proj = ubo.proj;
    }
    if (!isMatrixNull(ubo.view)) {
        view = ubo.view;
    }
    if (!isMatrixNull(ubo.model)) {
        model = ubo.model;
    }
    
    gl_Position = proj * view * model * vec4(position, 1.0);
    ourColor = color;
}