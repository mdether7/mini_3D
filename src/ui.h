#ifndef MINI_UI_H
#define MINI_UI_H

#include <stdbool.h>

#include "shader.h"

extern ProgramType ui_g_selected_shader;
extern bool        ui_g_blending_enabled;

typedef struct {

    float position[3];
    float color[4];
    float uv[2];

    float width;
    float height;

} ui_element_data;

int ui_initialize(unsigned int count, ui_element_data* elements);
void ui_draw_quad(ui_element_data data);

#endif