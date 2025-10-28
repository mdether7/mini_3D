#version 460 core

uniform sampler2D renderer2d_texture;

out vec4 Frag_Color;

in vec4 color;
in vec2 uv;
in flat int tex_flag;

void main()
{
    if (tex_flag == 1)
        Frag_Color = texture(renderer2d_texture, uv);
    else
        Frag_Color = color;
}