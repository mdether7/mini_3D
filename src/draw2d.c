#include "draw2d.h"

#include <stdbool.h>
#include <assert.h>

#include <glad/glad.h>

#include "shader.h"
#include "data_geometry.h"

ProgramSlot     draw2d_g_selected_shader;

static GLuint   draw2d_g_quad_VAO;
static GLuint   draw2d_g_quad_VBO;
static GLuint   draw2d_g_quad_EBO;

static bool     draw2d_g_initialized = false;

void draw2d_set_program(ProgramSlot slot)
{
    draw2d_g_selected_shader = slot;
}

void draw2d_quad(float x, float y, float w, float h, float* color)
{
    assert(draw2d_g_initialized && "[DRAW2D]: not initialized!");

    shader_use_program(draw2d_g_selected_shader);

    glUniform2f(g_shader_programs[draw2d_g_selected_shader].u_locations[UNIFORM_POS_2D], x, y);
    glUniform2f(g_shader_programs[draw2d_g_selected_shader].u_locations[UNIFORM_SIZE_2D], w, h);
    glUniform4fv(g_shader_programs[draw2d_g_selected_shader].u_locations[UNIFORM_COLOR], 1, color);

    glBindVertexArray(draw2d_g_quad_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw2d_quad_textured(GLuint shader, GLuint texture, float x, float y, float w, float h)
{
    glUseProgram(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    



}

int draw2d_init(void)
{
    glGenVertexArrays(1, &draw2d_g_quad_VAO);
    glBindVertexArray(draw2d_g_quad_VAO);

    glGenBuffers(1, &draw2d_g_quad_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, draw2d_g_quad_VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(geo_quad_vertices), geo_quad_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); //position
    glEnableVertexAttribArray(1); //uv

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)(3 * sizeof(GLfloat)));

    glGenBuffers(1, &draw2d_g_quad_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, draw2d_g_quad_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)sizeof(geo_quad_indices), geo_quad_indices, GL_STATIC_DRAW);

    draw2d_g_initialized = true;
    return 0;
}

void draw2d_cleanup(void)
{
    if (draw2d_g_initialized) {
        glDeleteVertexArrays(1, &draw2d_g_quad_VAO);
        glDeleteBuffers(1, &draw2d_g_quad_VBO);
        glDeleteBuffers(1, &draw2d_g_quad_EBO);
        draw2d_g_initialized = false;
    }
}
