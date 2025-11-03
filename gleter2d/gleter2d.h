#ifndef GLETER_2D_H
#define GLETER_2D_H

#include <glad/glad.h>
#define STBTT_STATIC  // To avoid conflict with Nuklear for my project.
#include <stb/stb_truetype.h>

/////////////////////////////////////
// DEPENDENCIES                    
// glad, linmath.h, stb_truetype.h 

//////////////////////////
// FEATURES
// Load ttf fonts.
// Save textures as png.

//////////
// FONT 

typedef struct {
    stbtt_packedchar *packed_char_array;
    unsigned char* ttf_data;
    GLuint atlas;
    unsigned int num_chars;
} GLE2D_Font;

int gle2d_font_load_and_pack_atlas(GLE2D_Font* font, const char* path, float px_size);
void gle2d_font_render_text(const char* text, float x, float y);
void gle2d_font_cleanup(GLE2D_Font* font);

/////////
// MISC

// TODO: Add save to folder, assure folder is created, Add linux/windows compability.
// NOTES: Unbinds currently bound texture.
//        Requires texture format to be GL_RED (single channel, 8-bit).
int gle2d_misc_texture_save_to_disk_as_png(GLuint texture, const char* name);




#endif