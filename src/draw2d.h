#ifndef DRAW_2D_H
#define DRAW_2D_H

#include <stdbool.h>

#include "shader.h"

int  draw2d_init(void);
void draw2d_cleanup(void);
void draw2d_set_program(ProgramSlot slot);
void draw2d_quad(float x, float y, float w, float h, float* color);
void draw2d_quad_textured(GLuint shader, GLuint texture, float x, float y, float w, float h, int w_width, int w_height);

#endif