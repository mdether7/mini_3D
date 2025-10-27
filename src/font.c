#include "font.h"

#include <assert.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>

#define MINI_MAX(a,b)  (((a) > (b)) ? (a) : (b))

Font* font_create(const char* path, unsigned int size)
{
    // FreeType
    FT_Library ft; 
    if (FT_Init_FreeType(&ft))
        return NULL;
    FT_Face face; 
    if (FT_New_Face(ft, path, 0, &face))
        return NULL;

    // Set rendering size to 48 pixels tall
    FT_Set_Pixel_Sizes(face, 0, size); // but glyphs might be higher than that.

    // calculate max width/height for atlas.
    unsigned int atlas_w = 0;
    unsigned int atlas_h = 0;
    for (unsigned char c = 32; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            util_print_n_flush("Failed to load glyph!");
            continue;
        }
        atlas_w += face->glyph->bitmap.width;
        atlas_h = MINI_MAX(atlas_h, face->glyph->bitmap.rows);
    }

    // create NULL texture.
    GLuint atlas_texture;
    glGenTextures(1, &atlas_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_w, atlas_h, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // How OpenGL interprets YOUR buffer when uploading TO GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int x_pos = 0; // populate atlas
    for (unsigned char c = 33; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
#ifdef DEBUG
            util_print_n_flush("[FONT] Failed to load glyph! %c", c);
#endif
            continue;
        }

        glTexSubImage2D(GL_TEXTURE_2D,
                        0, 
                        x_pos, 0,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        GL_RED, GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer);

        x_pos += face->glyph->bitmap.width;
    }

    // Just to be sure theres no mismatch.
    {GLint width, height; // Dunno there might.
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    assert(atlas_w == width && atlas_h == height);}

    Font* font = malloc(sizeof(*font));
    if (!font) {
        return NULL;
    }





}



// FUTURE IDEAS.
// // Czech, Polish, Turkish, etc.
// const char* test_chars = "ÁÄČĎÉĚÍŇÓŘŠŤÚŮÝŽáäčďéěíňóřšťúůýžĄĆĘŁŃÓŚŹŻąćęłńóśźżÇĞİÖŞÜçğıöşü";

// for (const char* p = test_chars; *p; ) {
//     // UTF-8 decoding here...
//     FT_Load_Char(face, codepoint, FT_LOAD_RENDER);
//     // Some will definitely exceed 48px!
// }
