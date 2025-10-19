#include "ui.h"

#include <stdbool.h>
#include <assert.h>

#include <glad/glad.h>

#include "shader.h"
#include "data_geometry.h"

ProgramType ui_g_selected_shader;
bool        ui_g_blending_enabled;

GLuint      ui_g_quad_VAO;
GLuint      ui_g_quad_VBO;
GLuint      ui_g_quad_EBO;

int            
ui_initialize(unsigned int count, ui_element_data* elements)
{ (void)count; // for now.

    // quad initialization leaves here for now.
    glGenVertexArrays(1, &ui_g_quad_VAO);
    glBindVertexArray(ui_g_quad_VAO);

    glGenBuffers(1, &ui_g_quad_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, ui_g_quad_VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(geo_quad_vertices), geo_quad_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); //position
    glEnableVertexAttribArray(1); //uv

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (void*)(3 * sizeof(GLfloat)));

    glGenBuffers(1, &ui_g_quad_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_g_quad_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)sizeof(geo_quad_indices), geo_quad_indices, GL_STATIC_DRAW);

    elements[0].position[0] = 50.0f;
    elements[0].position[1] = 50.0f;
    elements[0].position[2] = 0.0f; // z value always 0.0f for ui??

    elements[0].color[0]    = 1.0f;
    elements[0].color[1]    = 0.5f;
    elements[0].color[2]    = 1.0f;
    elements[0].color[3]    = 0.5f;

    elements[0].uv[0]       = 0.0f;
    elements[0].uv[1]       = 1.0f;

    elements->width         = 200.0f;
    elements->height        = 50.0f;

    return 0;
};

void ui_begin_frame(float current_time, int width, int height)
{
    shader_use_program(ui_g_selected_shader);

    glUniform1f(g_shader_programs[ui_g_selected_shader].u_locations[UNIFORM_TIME], current_time);
    glUniform2i(g_shader_programs[ui_g_selected_shader].u_locations[UNIFORM_RESOLUTION], 
                (GLint)width, (GLint)height);

    glEnable(GL_BLEND);
}

void ui_end_frame(void)
{
    glDisable(GL_BLEND);
    glUseProgram(0);
}

void ui_draw_quad(ui_element_data data)
{     
    assert((data.width >= 0 && data.height >= 0) && "[UI]: width/height negative!");

    glBindVertexArray(ui_g_quad_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}