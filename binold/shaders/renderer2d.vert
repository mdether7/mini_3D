#version 460 core

layout(location = 0) in vec2 in_position;

uniform mat4 u_projection;
uniform mat4 u_model;
uniform vec4 u_color;
uniform int  u_use_texture;

out vec4 color;
out vec2 uv;
out int  tex_flag;


void main()
{
    gl_Position = u_projection * u_model * vec4(in_position, 0.0f, 1.0f);
    tex_flag = u_use_texture;
    
    if (u_use_texture == 1) {
        uv = vec2(in_position.x,  in_position.y);
    } else {
        color = u_color;
    }   
         
}

