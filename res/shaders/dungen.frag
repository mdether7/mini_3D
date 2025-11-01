#version 430 core

in VS_OUT {
    vec4 color_to_frag;
} vs_in;

// output to framebuffer.
out vec4 color;

void main(void)
{
    color = vs_in.color_to_frag;
}