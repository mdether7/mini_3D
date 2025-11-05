#ifndef GLETER_2D_H
#define GLETER_2D_H

#include <glad/glad.h>
#include <linmath/linmath.h>
#define STBTT_STATIC  // To avoid conflict with Nuklear for my project.
#include <stb/stb_truetype.h>

/////////////////////////////////////
// DEPENDENCIES                    
// glad, linmath.h, stb_truetype.h 

//////////////////////////
// FEATURES
// Load ttf fonts.
// Render text using those fonts.
// Save textures as png.

// TODO wglGetCurrentContext/Linux verion to chceck if opengl started.
// TODO add color as parameter to text
// TODO add rendering textured quads.
// TODO add basic texture functionality.

//////////
// GLE2D

// Run this after openGL is initialized.
int gle2d_init(void);
// Run this at least once before rendering to update 2d projection matrix.
// And after every resolution change.
void gle2d_update_rendering_area(int viewport_width, int viewport_height);
void gle2d_shutdown(void);

/////////////
// TEXTURES

typedef struct {
    GLuint id;
} GLE2D_Texture;

///////////
// Shapes

void gle2d_shapes_draw_quad(float x, float y, float w, float h, vec4 color, GLuint texture);

//////////
// FONT 
typedef struct {
    stbtt_packedchar *packed_char_array;
    unsigned char* ttf_data;
    GLuint atlas;
    unsigned int num_chars;
} GLE2D_Font;

int gle2d_font_create(GLE2D_Font* font, const char* path, float px_size);
void gle2d_font_render_text(const GLE2D_Font* font, const char *text, float x, float y);
void gle2d_font_destroy(GLE2D_Font* font);

/////////
// MISC

// TODO: Add save to folder, assure folder is created, Add linux/windows compability.
// NOTES: Unbinds currently bound texture.
//        Requires texture format to be GL_RED (single channel, 8-bit).
int gle2d_misc_texture_save_to_disk_as_png(GLuint texture, const char* name);

#endif