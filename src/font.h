#ifndef MINI_FONT_H
#define MINI_FONT_H

#include "mini_types.h"

typedef struct {

    TextureID atlas;
    
} Font;
                                    // change type.
Font* font_create(const char* path, int size);
void  font_render(const char* text, float pos_x, float pos_y);

#endif