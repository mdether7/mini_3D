#version 460 core

out vec3 color;

in vec3 fragment_color;

void main()
{
    color = fragment_color;
    color *= 1.0f;
}