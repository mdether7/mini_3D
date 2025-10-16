#version 460 core

out vec3 color;

in vec3 fragment_color;

uniform float u_time;

void main()
{
    color = fragment_color * u_time;
}