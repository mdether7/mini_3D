#include "renderer2d.h"

#include <glad/glad.h>
#include "../math_helpers.h"

Render2DContext renderer2d = {
    .program   = 0,
    .ortho    = MAT4x4_IDENTITY,                                        
    .quad_vbo = 0,                           
    .quad_vao = 0,                           
    .quad_ebo = 0,
    .u_use_texture = -1,
    .u_projection  = -1,
    .u_model       = -1,
    .u_color       = -1,
};

int renderer2d_init(GLuint program, int window_width, int window_height)
{
    static float quad_vertices[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f, 
        1.0f, 0.0f,
    };

    static unsigned int quad_indices[] = {
        0, 3, 1,
        2, 1, 3,
    };

    glGenVertexArrays(1, &renderer2d.quad_vao);
    glBindVertexArray(renderer2d.quad_vao);

    glGenBuffers(1, &renderer2d.quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer2d.quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); //position and uv encoded.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &renderer2d.quad_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer2d.quad_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW); 

    renderer2d.program = program;
    renderer2d.u_use_texture = glGetUniformLocation(renderer2d.program, "u_use_texture");
    renderer2d.u_model = glGetUniformLocation(renderer2d.program, "u_model");
    renderer2d.u_projection = glGetUniformLocation(renderer2d.program, "u_projection");
    renderer2d.u_color = glGetUniformLocation(renderer2d.program, "u_color");

    renderer2d_update_ortho(window_width, window_height);

    return 0;
}

void renderer2d_update_ortho(int window_width, int window_height)
{
    mat4x4_ortho(renderer2d.ortho, 0, window_width, 0, window_height, -1, 1);
}

void renderer2d_draw_quad(float x, float y, float w, float h, float color[4])
{
    mat4x4 model = MAT4x4_IDENTITY;
    mat4x4_translate(model, x, y, 0);
    mat4x4_scale_aniso(model, model, w, h, 1);

    glDisable(GL_DEPTH_TEST); 
    glDisable(GL_CULL_FACE);

    glUseProgram(renderer2d.program);
    glUniform4fv(renderer2d.u_color, 1, color);
    glUniformMatrix4fv(renderer2d.u_model, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(renderer2d.u_projection, 1, GL_FALSE, &renderer2d.ortho[0][0]);
    glUniform1i(renderer2d.u_use_texture, 0); // don't use texture.

    glBindVertexArray(renderer2d.quad_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void renderer2d_draw_quad_textured(GLuint texture, float x, float y, float w, float h)
{
    mat4x4 model = MAT4x4_IDENTITY;
    mat4x4_translate(model, x, y, 0);
    mat4x4_scale_aniso(model, model, w, h, 1);

    glDisable(GL_DEPTH_TEST); 
    glDisable(GL_CULL_FACE);

    glUseProgram(renderer2d.program);
    glUniformMatrix4fv(renderer2d.u_model, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(renderer2d.u_projection, 1, GL_FALSE, &renderer2d.ortho[0][0]);
    glUniform1i(renderer2d.u_use_texture, 1); // use texture.

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(renderer2d.quad_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void renderer2d_cleanup()
{
    glDeleteBuffers(1, &renderer2d.quad_vbo);
    glDeleteBuffers(1, &renderer2d.quad_ebo);
    glDeleteVertexArrays(1, &renderer2d.quad_vao);
}


