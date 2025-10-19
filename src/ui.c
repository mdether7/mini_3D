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
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(geo_cube_vertices), geo_quad_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // only enable position.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glGenBuffers(1, &ui_g_quad_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ui_g_quad_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)sizeof(geo_quad_indices), geo_quad_indices, GL_STATIC_DRAW);

    elements[0].position[0] = 50.0f;
    elements[0].position[1] = 50.0f;
    elements[0].position[2] = 0.0f; // z value always 0.0f for ui??

    elements[0].color[0]    = 0.5f;
    elements[0].color[1]    = 0.5f;
    elements[0].color[2]    = 0.5f;
    elements[0].color[3]    = 0.5f;

    elements[0].uv[0]       = 0.0f;
    elements[0].uv[1]       = 1.0f;

    elements->width         = 200.0f;
    elements->height        = 50.0f;

    return 0;
};

void ui_draw_quad(ui_element_data data)
{     
    assert((data.width >= 0 && data.height >= 0) && "[UI]: width/height negative!");

    if (ui_g_blending_enabled)
        glEnable(GL_BLEND);

    shader_use_program(ui_g_selected_shader);




    if (ui_g_blending_enabled)
        glDisable(GL_BLEND);
}