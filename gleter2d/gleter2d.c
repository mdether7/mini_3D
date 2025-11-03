#include "gleter2d.h"

#include <stdio.h>
#include <assert.h>

#include <glad/glad.h>
#include <linmath/linmath.h>
#define STBTT_STATIC // To avoid conflict with Nuklear for my project.
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define GLE2D_DEAFULT_SHADER_VERSION "#version 430 core"
#define GLE2D_FONT_TTF_BUFFER_SIZE 1<<20 // 1 MiB
#define GLE2D_FONT_TTF_PIXEL_HEIGHT 64
#define GLE2D_FONT_BITMAP_SIZE 1024*1024 // 1 MiB

#define GLE2D_MINMAX(a,b)  (((a) > (b)) ? (a) : (b))

// Do only what needs to be done, build up your app //
// from smaller, simple, concise functions.         //

//////////
// FONT
GLuint font_atlas;

typedef struct {
   unsigned short x0;
   unsigned short y0;
   unsigned short x1;
   unsigned short y1;
   float xoff;
   float yoff;
   float xadvance;
} GLE2D_BakedCharData;

// g_default_font.glyphs[c].size[0] =    face->glyph->bitmap.width;
// g_default_font.glyphs[c].size[1] =    face->glyph->bitmap.rows;
// g_default_font.glyphs[c].bearing[0] = face->glyph->bitmap_left;
// g_default_font.glyphs[c].bearing[1] = face->glyph->bitmap_top;
// g_default_font.glyphs[c].advance =    face->glyph->advance.x;

GLE2D_BakedCharData cdata[96]; // ASCII 32..126 is 95 glyphs

int gle2d_font_load_ttf_form_file(const char* path, float pixel_height)
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
    //stbtt_BakeFontBitmap(ttf_buffer, 0, pixel_height, temp_bitmap, 1024, 1024, 32, 96, cdata);

    glGenTextures(1, &font_atlas);
    glBindTexture(GL_TEXTURE_2D, font_atlas);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1024,1024,0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

int gle2d_font_load_ttf_form_file_v2(const char* path)
{
    assert(path);
    FILE* f = fopen(path, "rb");
    if (!f) {
        return 1;
    }

    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    fseek(f, 0L, SEEK_SET);

    if (size == 0) {
        fclose(f);
        return 1;
    }

    unsigned char* ttf_buffer = malloc(size);
    size_t bytes_read = fread(ttf_buffer, 1, GLE2D_FONT_TTF_BUFFER_SIZE, f);
    fclose(f);

    if (bytes_read == 0) {
        return 1;
    }

    if (gle2d_font_load_ttf_from_datav2(ttf_buffer)) {
        return 1;
    }

    return 0;
}

unsigned char* gle2d_font_load_into_memory(const char* path)
{
    assert(path);
    unsigned char* data = NULL;

    FILE* f = fopen(path, "rb");
    if (!f) {
        return NULL;
    }

    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    if (size <= 0) {
        fclose(f);
        return NULL;
    }
    
    data = malloc(size);
    if (!data) {
        fclose(f);
        return NULL;
    }

    size_t bytes_read = fread(data, 1, size, f);
    fclose(f);
    if (bytes_read != (size_t)size) {  // Should read exactly 'size' bytes
        free(data);
        return NULL;
    }

    return data;
}

int gle2d_font_load_and_pack_atlas(GLE2D_Font* font, const char* path, float px_size)
{
    assert(font && path);
    assert(px_size > 0);

    // GLE2D_Font //
    stbtt_packedchar* packed_char_array = NULL;
    unsigned char* ttf_data = NULL;
    GLuint atlas = 0;
    int num_char = 95; //! For now at least.
    //------------//

    packed_char_array = malloc(sizeof(*packed_char_array) * num_char);
    if (!packed_char_array) {
        return 1;
    }
    
    ttf_data = gle2d_font_load_into_memory(path);
    if (!ttf_data) {
        free(packed_char_array);
        return 1;
    }

    stbtt_fontinfo font_info;                                         
    if (!stbtt_InitFont(&font_info, ttf_data, stbtt_GetFontOffsetForIndex(ttf_data, 0))) {
        free(ttf_data);
        free(packed_char_array);
        return 1;
    }

    unsigned char* bitmap = malloc(GLE2D_FONT_BITMAP_SIZE);
    if (!bitmap) {
        free(packed_char_array);
        free(ttf_data);
        return 1;
    }
    memset(bitmap, 0, GLE2D_FONT_BITMAP_SIZE); // set to black

    stbtt_pack_context spc;                                              // Thightly packed
    if (!stbtt_PackBegin(&spc, bitmap, 1024, 1024, 0, 1, NULL)) {
        free(packed_char_array);
        free(ttf_data);
        free(bitmap);
        return 1;
    } 
    //float scale = stbtt_ScaleForPixelHeight(&font_info, px_size);              
    if (!stbtt_PackFontRange(&spc, ttf_data, 0, px_size, 32, 95, packed_char_array)) {
        free(packed_char_array);
        free(ttf_data);
        free(bitmap);
        return 1;
    }
    stbtt_PackEnd(&spc);

    glGenTextures(1, &atlas);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 1024, 1024, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    free(bitmap);

    font->packed_char_array = packed_char_array;
    font->ttf_data = ttf_data;
    font->atlas = atlas;
    font->num_chars = num_char;

    return 0;
}

// stbtt_GetPackedQuad

// Clear this, use PACK.
int gle2d_font_load_ttf_from_datav2(const unsigned char* data)
{
    assert(data);

    stbtt_fontinfo font_info;
    if (!stbtt_InitFont(&font_info, data, stbtt_GetFontOffsetForIndex(data, 0))) {
        return 1;
    }

    float scale = stbtt_ScaleForPixelHeight(&font_info, GLE2D_FONT_TTF_PIXEL_HEIGHT);

    // Find atlas dimensions (1 ROW)
    int atlas_w = 0;
    int atlas_h = 0; 
    for (int i = 32; i <= 126; i++) {
        int x0, y0, x1, y1;
        stbtt_GetCodepointBitmapBox(&font_info, i, scale, scale, &x0, &y0, &x1, &y1);

        cdata[i - 32].x0 = x0;
        cdata[i - 32].y0 = y0;
        cdata[i - 32].x1 = x1;
        cdata[i - 32].y1 = y1;

        atlas_w += x1 - x0;
        atlas_h = GLE2D_MINMAX(atlas_h, y1 - y0);
    }

    glGenTextures(1, &font_atlas);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font_atlas);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_w, atlas_h, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // How OpenGL interprets YOUR buffer when uploading TO GPU
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int x_pos = 0;
    unsigned char* bitmap;
    for (int i = 32; i <= 126; i++) {
        int w, h, xoff, yoff;
        bitmap = stbtt_GetCodepointBitmap(&font_info, scale, scale, i, &w, &h, &xoff, &yoff);
        glTexSubImage2D(GL_TEXTURE_2D, 0, x_pos, 0, w, h, GL_RED, GL_UNSIGNED_BYTE, bitmap);
        x_pos += w;
    }
    stbtt_FreeBitmap(bitmap, NULL);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // Back to default.
    glBindTexture(GL_TEXTURE_2D, 0);

    return 0;
}

GLuint gle2d_font_get_font_texture(void)
{
    return font_atlas;
}

void gle2d_font_render_text(const char* text, float x, float y)
{
    // glDisable(GL_DEPTH_TEST);
}

void gle2d_font_cleanup(void)
{
    glDeleteTextures(1, &font_atlas);
}

/////////
// MISC

int gle2d_misc_texture_save_to_disk_as_png(GLuint texture, const char* name)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    GLint width, height, internal_format;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internal_format);

    if (internal_format != GL_RED && internal_format != GL_R8) {
        glBindTexture(GL_TEXTURE_2D, 0);
        return 1; // not 1 byte per pixel.
    } 
    
    unsigned char* pixels = malloc(width * height);
    if (!pixels) {
        glBindTexture(GL_TEXTURE_2D, 0);
        return 1;
    }

    // How OpenGL writes to YOUR buffer when downloading FROM GPU
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, (void*)pixels);
    glPixelStorei(GL_PACK_ALIGNMENT, 4); // restore to openGL default = 4.

    int result = stbi_write_png(name ,width, height, 1, pixels, width);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    free(pixels);

    return result ? 0 : 1;
}





