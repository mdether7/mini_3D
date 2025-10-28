#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include <glad/glad.h>
#include <linmath/linmath.h>

#include "math_helpers.h"

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

Render2DContext renderer2d = {
    .program   = 0, // cus UNSIGNED INT!!! TODO: get your hands dirty with
    .ortho    = MAT4x4_IDENTITY,             // debugging on linux.
    .quad_vao = 0,                           // glfwGetWindowSize;                           
    .quad_vbo = 0,                           // glfwGetFramebufferSize;
    .quad_ebo = 0,
    .u_use_texture = -1,
    .u_projection  = -1,
    .u_model       = -1,
    .u_color       = -1,
};

#endif