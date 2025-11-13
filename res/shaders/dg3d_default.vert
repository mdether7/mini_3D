#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

layout (std140, binding = 0) uniform uBlockMatrices {
    mat4 view;
    mat4 projection;
};

uniform mat4 uModel;

void main() {
    gl_Position = projection * view * uModel * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}