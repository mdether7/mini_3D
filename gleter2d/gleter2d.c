#include "gleter2d.h"

#include <stdio.h>
#include <assert.h>

#include <glad/glad.h>
#include <linmath/linmath.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>

#define GLETER2D_DEAFULT_SHADER_VERSION "#version 430 core"

//////////
// FONT //
GLuint font_tex;
stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs

int gle2d_font_read_ttf_form_file(const char* path, float pixel_height)
{
    assert(path);

    FILE* f = fopen(path, "rb");
    if (!f) {
        return 1;
    }

    unsigned char ttf_buffer[1<<20]; 
    size_t bytes_read = fread(ttf_buffer, 1, 1<<20, f);
    fclose(f);

    if (bytes_read == 0) {
        return 1;
    }

    unsigned char temp_bitmap[1024*1024];
    stbtt_BakeFontBitmap(ttf_buffer,0, pixel_height, temp_bitmap, 1024, 1024, 32, 96, cdata);

    glGenTextures(1, &font_tex);
    glBindTexture(GL_TEXTURE_2D, font_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1024,1024,0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}




