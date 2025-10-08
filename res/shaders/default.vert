#version 460 core

layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(vertexPosition_modelspace, 1.0f);
}