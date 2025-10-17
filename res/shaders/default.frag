#version 460 core

out vec4 color;

in vec3 v_color;
in vec2 v_texture;

uniform float u_time;
uniform sampler2D dirt_texture;

void main()
{
    // color = vec4(v_color, 1.0f);
    color = texture(dirt_texture, v_texture);
}
