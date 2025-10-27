#include "draw2d.h"

#include <stdbool.h>
#include <assert.h>

#include <glad/glad.h>
#include <linmath/linmath.h>

#include "shader.h"
#include "math_helpers.h"
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

// TODO THIS SHADER IMPLEMENTATION NEED TO GO TO HELLLL
void draw2d_quad_textured(GLuint shader, GLuint texture, float x, float y, float w, float h, int w_width, int w_height) {
    glDisable(GL_DEPTH_TEST); 
    glDisable(GL_CULL_FACE);
    glUseProgram(shader);

    GLint tex_loc = glGetUniformLocation(shader, "draw2d_texture");
    glUniform1i(tex_loc, 0);  // Tell shader to use GL_TEXTURE0

    mat4x4 model = MAT4x4_IDENTITY;
    mat4x4_translate(model, x, y, 0);
    mat4x4_scale_aniso(model, model, w, h, 1);

    mat4x4 ortho = MAT4x4_IDENTITY;
    mat4x4_ortho(ortho, 0, w_width, 0, w_height, -1, 1);

    glUniformMatrix4fv(g_shader_programs[PROGRAM_SLOT_3].u_locations[UNIFORM_PROJECTION], 1, GL_FALSE, &ortho[0][0]);
    glUniformMatrix4fv(g_shader_programs[PROGRAM_SLOT_3].u_locations[UNIFORM_MODEL], 1, GL_FALSE, &model[0][0]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(draw2d_g_quad_VAO); // this is just plain wrong....
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
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
