#version 460 core

out vec4 color;

in vec2 v_uv;

uniform sampler2D draw2d_texture;

void main()
{  
    color = texture(draw2d_texture, v_uv);
}
