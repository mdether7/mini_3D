#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

out vec4 v_color;

uniform ivec2 u_resolution;
uniform vec2  u_pos;
uniform vec2  u_size;
uniform vec4  u_color;

void main()
{
//     static const Vertex2D geo_quad_vertices[] = {
//     {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // bottom-left
//     {{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, // top-left
//     {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}}, // top-right
//     {{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // bottom-right
// };

    // draw2d_quad(10.0f, 10.0f, 1000.0f, 20.0f, color);
    vec2 pixel_pos = in_position.xy * u_size + u_pos;
    vec2 ndc = (pixel_pos / u_resolution) * 2.0 - 1.0;

    ndc.y = -ndc.y;

    gl_Position = vec4(ndc.x, ndc.y, -1.0f, 1.0);
    v_color = u_color;
}