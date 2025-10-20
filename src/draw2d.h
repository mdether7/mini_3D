#ifndef MINI_UI_H
#define MINI_UI_H

#include <stdbool.h>

#include "shader.h"

extern ProgramSlot draw2d_g_selected_shader;

int  draw2d_init(void);
void draw2d_set_program(ProgramSlot slot);
void draw2d_quad(float x, float y, float w, float h, float* color);

#endif