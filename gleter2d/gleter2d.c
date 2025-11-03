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
#define GLE2D_FONT_BITMAP_SIZE 1024

// Do only what needs to be done, build up your app //
// from smaller, simple, concise functions.         //

//////////
// FONT

static unsigned char* gle2d_font_load_into_memory(const char *path)
{
    assert(path);
    unsigned char *data = NULL;

    FILE *f = fopen(path, "rb");
    if (!f)
    {
        return NULL;
    }

    fseek(f, 0L, SEEK_END);
    long size = ftell(f);
    fseek(f, 0L, SEEK_SET);
    if (size <= 0)
    {
        fclose(f);
        return NULL;
    }

    data = malloc(size);
    if (!data)
    {
        fclose(f);
        return NULL;
    }

    size_t bytes_read = fread(data, 1, size, f);
    fclose(f);
    if (bytes_read != (size_t)size)
    { // Should read exactly 'size' bytes
        free(data);
        return NULL;
    }

    return data;
}

int gle2d_font_load_and_pack_atlas(GLE2D_Font *font, const char *path, float px_size)
{
    assert(font && path);
    assert(px_size > 0);

    // Function specific //
    int result = 1;

    stbtt_fontinfo font_info;
    stbtt_pack_context spc;
    unsigned char *bitmap = NULL;
    //-------------------//

    // GLE2D_Font //
    stbtt_packedchar *packed_char_array = NULL;
    unsigned char *ttf_data = NULL;
    GLuint atlas = 0;
    int num_char = 95; //! HARDCODED
    //------------//

    packed_char_array = malloc(sizeof(*packed_char_array) * num_char);
    if (!packed_char_array) {
        goto cleanup;
    }

    ttf_data = gle2d_font_load_into_memory(path);
    if (!ttf_data) {
        goto cleanup;
    }

    if (!stbtt_InitFont(&font_info, ttf_data, stbtt_GetFontOffsetForIndex(ttf_data, 0))) {
        goto cleanup;
    }

    bitmap = malloc(GLE2D_FONT_BITMAP_SIZE * GLE2D_FONT_BITMAP_SIZE);
    if (!bitmap) {
        goto cleanup;
    }
    memset(bitmap, 0, GLE2D_FONT_BITMAP_SIZE * GLE2D_FONT_BITMAP_SIZE); // set to black

    if (!stbtt_PackBegin(&spc, bitmap, GLE2D_FONT_BITMAP_SIZE, GLE2D_FONT_BITMAP_SIZE, 0, 1, NULL)) {
        goto cleanup;
    }
    if (!stbtt_PackFontRange(&spc, ttf_data, 0, px_size, 32, 95, packed_char_array)) {
        goto cleanup;
    }
    stbtt_PackEnd(&spc);

    glGenTextures(1, &atlas);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, GLE2D_FONT_BITMAP_SIZE, GLE2D_FONT_BITMAP_SIZE, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    font->packed_char_array = packed_char_array;
    font->ttf_data = ttf_data;
    font->atlas = atlas;
    font->num_chars = num_char;
    result = 0;

cleanup:
    if (result != 0) {
        free(bitmap);
        free(ttf_data);
        free(packed_char_array);
    } else {
        free(bitmap);
    }
    return result;
}

// stbtt_GetPackedQuad ?

void gle2d_font_render_text(const char *text, float x, float y)
{
    // glDisable(GL_DEPTH_TEST);
}

void gle2d_font_cleanup(GLE2D_Font* font)
{
    if (!font) return;

    free(font->packed_char_array);
    font->packed_char_array = NULL;

    free(font->ttf_data);
    font->ttf_data = NULL;

    if (font->atlas) {
        glDeleteTextures(1, &font->atlas);
        font->atlas = 0;
    }

    font->num_chars = 0;
}


/////////
// MISC

int gle2d_misc_texture_save_to_disk_as_png(GLuint texture, const char *name)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    GLint width, height, internal_format;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internal_format);

    if (internal_format != GL_RED && internal_format != GL_R8)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        return 1; // not 1 byte per pixel.
    }

    unsigned char *pixels = malloc(width * height);
    if (!pixels)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        return 1;
    }

    // How OpenGL writes to YOUR buffer when downloading FROM GPU
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, (void *)pixels);
    glPixelStorei(GL_PACK_ALIGNMENT, 4); // restore to openGL default = 4.

    int result = stbi_write_png(name, width, height, 1, pixels, width);

    glBindTexture(GL_TEXTURE_2D, 0);
    free(pixels);

    return result ? 0 : 1;
}
