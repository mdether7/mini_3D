#version 460 core

out vec4 color;

uniform float u_time;
uniform ivec2 u_resolution;

void main()
{
    vec2 coords_normalized = gl_FragCoord.xy / vec2(u_resolution);
    color = vec4(vec2(coords_normalized.xy), 0.0f, 0.2f); // really low alpha
}                                                          // but it's there!