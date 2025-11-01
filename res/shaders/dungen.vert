#version 460 core

layout (location = 0) in vec2 offset;
layout (location = 1) in vec4 color;

out VS_OUT { // something simmilar to struct.
    vec4 color_to_frag;
} vs_out;

void main(void)
{
    const vec2 positions[] = {
        vec2(0.0f, 0.0f),
        vec2(0.5f, 0.5f),
        vec2(0.5f, -0.5f),
    };

    gl_Position = vec4(positions[gl_VertexID] + offset, 0.5, 1.0);
    vs_out.color_to_frag = color;
}