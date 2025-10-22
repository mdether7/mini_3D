#version 460 core

out vec4 color;

uniform float u_time;
uniform ivec2 u_resolution;

void main()
{
    vec2 flipped_coord = vec2(gl_FragCoord.x, u_resolution.y - gl_FragCoord.y);
    vec2 coords_normalized = flipped_coord / vec2(u_resolution);
    color = vec4(vec2(coords_normalized.xy), 0.0f, 0.8f); // really low alpha
}                                                         // but it's there!