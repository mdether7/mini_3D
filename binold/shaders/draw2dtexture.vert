#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

uniform mat4 u_projection;
uniform mat4 u_model;

out vec2 v_uv;

void main()
{
    gl_Position =  u_projection * u_model * vec4(in_position, 1.0f);
    v_uv = in_uv;
}