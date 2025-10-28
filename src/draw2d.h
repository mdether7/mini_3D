#ifndef DRAW_2D_H
#define DRAW_2D_H

#include <stdbool.h>

#include "shader.h"
#include "math_helpers.h"

typedef GLint uniform;

typedef struct {
    GLuint  shader;
    mat4x4  ortho;

    GLuint  quad_vao;
    GLuint  quad_vbo;
    GLuint  quad_ebo;
    
    uniform u_projection;
    uniform u_texture;
    uniform u_model;
} Render2DContext;

// Render2DContext renderer2d = {
//     .shader   = 0, // cus UNSIGNED INT!!! TODO: get your hands dirty with
//     .ortho    = MAT4x4_IDENTITY,             // debugging on linux.
//     .quad_vao = 0,                         
//     .quad_vbo = 0,
//     .quad_ebo = 0,
//     .u_projection = -1,
//     .u_model      = -1,
//     .u_texture    = -1,
// };

int  draw2d_init(void);
void draw2d_cleanup(void);
void draw2d_set_program(ProgramSlot slot);
void draw2d_quad(float x, float y, float w, float h, float* color);
void draw2d_quad_textured(GLuint shader, GLuint texture, float x, float y, float w, float h, int w_width, int w_height);

#endif