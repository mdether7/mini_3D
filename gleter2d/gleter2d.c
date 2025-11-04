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

#define GLE2D_DEAFULT_SHADER_VERSION "#version 460 core\n"

#define GLE2D_FONT_ATLAS_SIZE 1024
#define GLE2D_FONT_FIRST_CHAR 32
#define GLE2D_FONT_LAST_CHAR 95

//////////////////////////////////////////////////////
// Do only what needs to be done, build up your app //
// from smaller, simple, concise functions.         //
//////////////////////////////////////////////////////

// GLE2D context //
GLuint gle2d_ctx_font_shader_program;
mat4x4 gle2d_ctx_projection_matrix;
GLuint gle2d_ctx_quad_vao;
GLuint gle2d_ctx_quad_vbo;
//---------------//

// GLE2D internal //
static unsigned char* gle2d_internal_font_load_into_memory(const char *path);
static GLuint         gle2d_internal_create_shader_from_data(const char* vertex, const char* fragment);
static int            gle2d_internal_shader_compile_error(GLuint shader);
static int            gle2d_internal_shader_program_link_error(GLuint shader);
//----------------//

int gle2d_init(void)
{
    const char* font_vertex_src = 
    GLE2D_DEAFULT_SHADER_VERSION
    "layout (location = 0) in vec4 vertex;\n"
    "out vec2 TexCoords;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    "   TexCoords = vertex.zw;\n"
    "}\n";

    const char* font_fragment_src = 
    GLE2D_DEAFULT_SHADER_VERSION
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "uniform sampler2D text;\n"
    "uniform vec3 textColor;\n"
    "void main()\n"
    "{\n"
    "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
    "   color = vec4(textColor, 1.0) * sampled;\n"
    "}\n";

    gle2d_ctx_font_shader_program = gle2d_internal_create_shader_from_data(font_vertex_src, font_fragment_src);
    if (gle2d_ctx_font_shader_program == 0) {
        return 1;
    }

    // First glGen...
    glGenVertexArrays(1, &gle2d_ctx_quad_vao);
    glGenBuffers(1, &gle2d_ctx_quad_vbo);
    glBindVertexArray(gle2d_ctx_quad_vao); // Start recording...
    glBindBuffer(GL_ARRAY_BUFFER, gle2d_ctx_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindVertexArray(0); // always unbind the VAO first, cus it remembers.
    glBindBuffer(GL_ARRAY_BUFFER, 0); // "Cleanup"...

    mat4x4_identity(gle2d_ctx_projection_matrix);
    return 0;
}

// The current drawable framebuffer dimensions (the values you pass to glViewport).
void gle2d_update_rendering_area(int viewport_width, int viewport_height)
{
    mat4x4_ortho(gle2d_ctx_projection_matrix, 0.0f, viewport_width, viewport_height, 0.0f, -1.0f, 1.0f);
}

void gle2d_cleanup(void)
{
    glDeleteProgram(gle2d_ctx_font_shader_program);
}

//////////
// FONT

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
    int num_char = GLE2D_FONT_LAST_CHAR;
    //------------//

    packed_char_array = malloc(sizeof(*packed_char_array) * num_char);
    if (!packed_char_array) {
        goto cleanup;
    }

    ttf_data = gle2d_internal_font_load_into_memory(path);
    if (!ttf_data) {
        goto cleanup;
    }

    if (!stbtt_InitFont(&font_info, ttf_data, stbtt_GetFontOffsetForIndex(ttf_data, 0))) {
        goto cleanup;
    }

    bitmap = malloc(GLE2D_FONT_ATLAS_SIZE * GLE2D_FONT_ATLAS_SIZE);
    if (!bitmap) {
        goto cleanup;
    }
    memset(bitmap, 0, GLE2D_FONT_ATLAS_SIZE * GLE2D_FONT_ATLAS_SIZE); // set to black

    if (!stbtt_PackBegin(&spc, bitmap, GLE2D_FONT_ATLAS_SIZE, GLE2D_FONT_ATLAS_SIZE, 0, 1, NULL)) {
        goto cleanup;
    }
    if (!stbtt_PackFontRange(&spc, ttf_data, 0, px_size, GLE2D_FONT_FIRST_CHAR, GLE2D_FONT_LAST_CHAR, packed_char_array)) {
        goto cleanup;
    }
    stbtt_PackEnd(&spc);

    glGenTextures(1, &atlas);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, GLE2D_FONT_ATLAS_SIZE, GLE2D_FONT_ATLAS_SIZE, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
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

void gle2d_font_render_text(const GLE2D_Font* font, const char *text, float x, float y)
{
    glUseProgram(gle2d_ctx_font_shader_program);
    glUniformMatrix4fv(glGetUniformLocation(gle2d_ctx_font_shader_program, "projection"), 1, GL_FALSE, &gle2d_ctx_projection_matrix[0][0]);
    glUniform3f(glGetUniformLocation(gle2d_ctx_font_shader_program, "textColor"), 0.5f, 0.8f, 0.2f);

    glBindVertexArray(gle2d_ctx_quad_vao);
    glBindTexture(GL_TEXTURE_2D, font->atlas);
    glBindBuffer(GL_ARRAY_BUFFER, gle2d_ctx_quad_vbo);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    float xpos = x;
    float ypos = y;
    while (*text != '\0') {
        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(font->packed_char_array,
                            GLE2D_FONT_ATLAS_SIZE,
                            GLE2D_FONT_ATLAS_SIZE,
                            *text - GLE2D_FONT_FIRST_CHAR,
                            &xpos, &ypos,
                            &q, 1);

        float char_vertices[] = {
            q.x0, q.y0, q.s0, q.t0,
            q.x0, q.y1, q.s0, q.t1,
            q.x1, q.y1, q.s1, q.t1,
            q.x1, q.y0, q.s1, q.t0,
            q.x0, q.y0, q.s0, q.t0,
            q.x1, q.y1, q.s1, q.t1,
        };
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(char_vertices), char_vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        text++;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
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

    if (internal_format != GL_RED && internal_format != GL_R8) {
        glBindTexture(GL_TEXTURE_2D, 0);
        return 1; // not 1 byte per pixel.
    }

    unsigned char *pixels = malloc(width * height);
    if (!pixels) {
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

/////////////
// Internal

static GLuint gle2d_internal_create_shader_from_data(const char* vertex, const char* fragment)
{
    assert(vertex && fragment);
    GLuint shader_program = 0;

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex_shader, 1, (const GLchar**)&vertex, NULL);
    glShaderSource(fragment_shader, 1, (const GLchar**)&fragment, NULL);
    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    if (gle2d_internal_shader_compile_error(vertex_shader)   > 0 ||
        gle2d_internal_shader_compile_error(fragment_shader) > 0) {

        glDeleteShader(vertex_shader);  
        glDeleteShader(fragment_shader);

        return shader_program;
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glDeleteShader(vertex_shader);  
    glDeleteShader(fragment_shader);

    if (gle2d_internal_shader_program_link_error(shader_program) > 0) {

        glDeleteProgram(shader_program);
        shader_program = 0;
        return shader_program;

    }

    return shader_program;

}

static int gle2d_internal_shader_compile_error(GLuint shader)
{
	GLint success;
	char info_log[1024] = {0};

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "[GLE2D][SHADER COMPILE] Log: %s", info_log);
		return 1;
	}
	return 0;
}

static int gle2d_internal_shader_program_link_error(GLuint shader)
{
    GLint success;
    char info_log[1024] = {0};

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "[GLE2D][SHADER LINK] Log: %s", info_log);
        return 1;
    }
    return 0;
}

static unsigned char* gle2d_internal_font_load_into_memory(const char *path)
{
    assert(path);
    unsigned char *data = NULL;

    FILE *f = fopen(path, "rb");
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
    if (bytes_read != (size_t)size) { // Should read exactly size bytes
        free(data);
        return NULL;
    }

    return data;
}
