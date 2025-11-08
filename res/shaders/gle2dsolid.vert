#version 460 core
layout (location = 0) in vec2 in_pos;
out vec4 vs_pass_color;
uniform vec4 u_color;
uniform mat4 u_model;
layout (std140) uniform u_BlockProjectionMatrix
{
   mat4 projection;
};
void main()
{
    gl_Position = projection * u_model * vec4(in_pos, 0.0, 1.0);
    vs_pass_color = u_color.rgba;
}