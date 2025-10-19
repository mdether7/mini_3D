#version 460 core

out vec4 color;

in vec2 v_uv;

uniform float u_time;
uniform sampler2D dirt_texture;

void main()
{
    color = texture(dirt_texture, v_uv);
}
