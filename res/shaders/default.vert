#version 460 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColorData;

out vec3 fragment_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(vertexPosition_modelspace, 1.0f);
    fragment_color = vertexColorData;
}