#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

out vec4 v_color;

uniform vec2 u_resolution;
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

                   // u_pos      // u_size
    // draw2d_quad(10.0f, 10.0f, 1000.0f, 20.0f, color);
    vec2 pixel_pos = in_position.xy * u_size + u_pos;


    // first vertex becomes => 10.0f, 10.0f,
    // second               => 10.0f, 30.0f,
    // third                => 1010.0f, 30.0f,
    // fourth               => 1010.0f, 10.0f,

    // Then normalization:
    // 1 => 10 / 1280 , 10 / 800   = 0.0078125, 0.0125
    // 2 => 10 / 1280 , 30 / 800   = 0.0078125, 0.0375
    // 3 => 1010 / 1280 , 30 / 800 = 0.7890625, 0.0375
    // 4 => 1010 / 1280 , 10 / 800 = 0.7890625, 0.0125

    // Then making it ndc?                    // NDC or CLIP SPACE??
    // 1 => (0.0078125, 0.0125) * 2.0 - 1.0 = −0.984375, −0.975
    // 2 => (0.0078125, 0.0375) * 2.0 - 1.0 = −0.984375, −0.925
    // 3 => (0.7890625, 0.0375) * 2.0 - 1.0 =  0.578125, -0.925
    // 4 => (0.7890625, 0.0125) * 2.0 - 1.0 =  0.578125, -0.975


    vec2 ndc = (pixel_pos / u_resolution) * 2.0 - 1.0;


// static const GLuint geo_quad_indices[] = {
//     0, 3, 1,
//     2, 1, 3,
// };

                        //-1 = near, +1 = far
    // if (gl_VertexID == 0)
    //     gl_Position = vec4(0.0f, -0.500, -1.0f, 1.0f);
    // else 
    ndc.y = -ndc.y;
    gl_Position = vec4(ndc.x, ndc.y, 0.0f, 1.0f);

    if (gl_VertexID == 0 || gl_VertexID == 3)
        v_color  = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    else
        v_color = u_color;
}