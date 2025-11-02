#ifndef GLETER_2D_H
#define GLETER_2D_H

#include <glad/glad.h>

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

} Font;

int gle2d_font_load_ttf_form_file(const char* path, float pixel_height);
int gle2d_font_load_ttf_form_file_v2(const char* path);
int gle2d_font_load_ttf_from_datav2(const unsigned char* data);
void gle2d_font_render_text(const char* text, float x, float y);
GLuint gle2d_font_get_font_texture(void);
void gle2d_font_cleanup(void);

/////////
// MISC

// TODO: Add save to folder, assure folder is created.
//       Add linux/windows compability.
// NOTES: Unbinds currently bound texture.
//        Requires texture format to be GL_RED (single channel, 8-bit).
int gle2d_misc_texture_save_to_disk_as_png(GLuint texture, const char* name);




#endif