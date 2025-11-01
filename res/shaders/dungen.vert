#version 460 core

void main(void)
{
    vec2 positions[] = {
        vec2(0.0f, 0.0f),
        vec2(1.0f, 1.0f),
    };

    gl_Position = vec4(positions[gl_VertexID], 0.5, 1.0);
}