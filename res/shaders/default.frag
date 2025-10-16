#version 460 core

precision highp float;

out vec3 color;

in vec3 fragment_color;

uniform float u_time;

void main()
{
    float fcx = gl_FragCoord.x;
    color = fragment_color;
}
