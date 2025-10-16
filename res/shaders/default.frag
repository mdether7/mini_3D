#version 460 core

out vec3 color;

in vec3 v_color;

uniform float u_time;

void main()
{
    color = v_color;
}
