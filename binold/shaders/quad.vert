#version 460 core

layout(location = 0) in vec3 in_position;

void main()
{
    vec2 ndc_pos = in_position.xy * 2.0f - 1.0f;
    ndc_pos.y = -ndc_pos.y;
    gl_Position = vec4(ndc_pos, -0.9f, 1.0f);
}