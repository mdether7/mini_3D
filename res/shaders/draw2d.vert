#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

uniform vec2 u_pos;
uniform vec2 u_size;
uniform vec4 u_color;

void main()
{
    gl_Position = vec4(in_position, 1.0f);
}