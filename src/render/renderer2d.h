#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include <glad/glad.h>
#include <linmath/linmath.h>

#include "../math_helpers.h"

typedef GLint uniform;

typedef struct {
    GLuint  program;
    mat4x4  ortho;

    GLuint  quad_vao;
    GLuint  quad_vbo;
    GLuint  quad_ebo;
    
    uniform u_use_texture;
    uniform u_projection;
    uniform u_model;
    uniform u_color;
} Render2DContext;

extern Render2DContext renderer2d;

int  renderer2d_init(GLuint program, int window_width, int window_height);
void renderer2d_update_ortho(int window_width, int window_height);
void renderer2d_draw_quad(float x, float y, float w, float h, float color[4]);
void renderer2d_draw_quad_textured(GLuint texture, float x, float y, float w, float h);
void renderer2d_cleanup();

#endif