#version 460 core

layout(location = 0) in vec2 in_position;

uniform mat4 u_projection;
uniform mat4 u_model;
uniform vec4 u_color;
uniform int  u_use_texture;

out vec2 v_uv;

void main()
{
    gl_Position = u_projection * u_model * vec4(in_position, 0.0f, 1.0f);
    v_uv = vec2(in_position.xy); 
}
