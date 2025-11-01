#version 430 core

// in VS_OUT {
//     vec4 color_to_frag;
// } vs_in;

in TES_OUT {
    vec4 color_to_frag;
} tes_in;

// output to framebuffer.
out vec4 color;

void main(void)
{
    color = tes_in.color_to_frag;
}