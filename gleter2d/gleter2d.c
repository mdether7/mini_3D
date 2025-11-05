#include "gleter2d.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <glad/glad.h>
#include <linmath/linmath.h>
#define STBTT_STATIC // To avoid conflict with Nuklear for my project.
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#define GLE2D_DEFAULT_SHADER_VERSION "#version 460 core\n"

#define GLE2D_FONT_ATLAS_SIZE 1024
#define GLE2D_FONT_MAX_TEXT_LEN 1024    
#define GLE2D_FONT_FIRST_CHAR 32
#define GLE2D_FONT_LAST_CHAR 95

//////////////////////////////////////////////////////
// Do only what needs to be done, build up your app //
// from smaller, simple, concise functions.         //
//////////////////////////////////////////////////////

typedef struct {
    mat4x4 projection_matrix;

    mat4x4 shapes_model_matrix;
    GLuint shapes_shader_program;
    GLint  shapes_loc_model;   
    GLint  shapes_loc_proj;    // This could be common \/
    GLint  shapes_loc_flag;
    GLint  shapes_loc_color;
    GLuint shapes_quad_vao;
    GLuint shapes_quad_vbo;
    GLuint shapes_quad_ebo;

    float* font_batch_buffer;
    GLuint font_shader_program;
    GLint  font_projection_loc; // With this... maybe?
    GLint  font_text_color_loc;
    GLuint font_quad_vao;
    GLuint font_quad_vbo;
} GLE2D_Context;

static GLE2D_Context context;

void util_print_mat4x4(const mat4x4 M)
{
    int i, j;
    fprintf(stdout, "-------\n");
	for(i=0; i<4; ++i) {
        for(j=0; j<4; ++j) {
            fprintf(stdout, "%.6f ", M[j][i]);
        }
        putchar('\n');
    }
    fprintf(stdout, "-------\n");
    fflush(stdout);
}

// GLE2D internal //
static unsigned char* gle2d_internal_font_load_into_memory(const char *path);
static GLuint         gle2d_internal_create_shader_from_data(const char* vertex, const char* fragment);
static int            gle2d_internal_shader_compile_error(GLuint shader);
static int            gle2d_internal_shader_program_link_error(GLuint shader);
//----------------//

// glIsTexture(texture) <= this seems useful.

int gle2d_init(void)
{
    // SHAPES //
    const char* shapes_vertex_src =
    GLE2D_DEFAULT_SHADER_VERSION
    "layout (location = 0) in vec2 in_pos;\n"
    "out vec4 vs_pass_color;\n"
    "out vec2 vs_pass_texture_uv;\n"
    "uniform vec4 u_color;\n"
    "uniform mat4 model;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n" // technically scaled_one_vertex of a quad??
    "   gl_Position = projection * model * vec4(in_pos, 0.0, 1.0);\n"
    "   vs_pass_color = u_color;\n"
    "   vs_pass_texture_uv = in_pos.xy;\n"
    "}\n";

    const char* shapes_fragment_src =
    GLE2D_DEFAULT_SHADER_VERSION
    "out vec4 FinalColor;\n"
    "in vec4 vs_pass_color;\n"
    "in vec2 vs_pass_texture_uv;\n"
    "uniform sampler2D quad_texture;\n"
    "uniform bool use_texture_flag;\n"
    "void main()\n"
    "{\n"
    "   FinalColor = vs_pass_color;\n"
    "}\n";

    context.shapes_shader_program = gle2d_internal_create_shader_from_data(shapes_vertex_src, shapes_fragment_src);
    if (context.shapes_shader_program == 0) {
        return 1;
    }
    context.shapes_loc_proj = glGetUniformLocation(context.shapes_shader_program, "projection");
    context.shapes_loc_model = glGetUniformLocation(context.shapes_shader_program, "model");
    context.shapes_loc_flag = glGetUniformLocation(context.shapes_shader_program, "use_texture_flag");
    context.shapes_loc_color = glGetUniformLocation(context.shapes_shader_program, "u_color");

    float quad[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f, 
        1.0f, 0.0f,
    };

    unsigned int quad_indices[] = {
        0, 3, 1,
        2, 1, 3,
    };

    // now create the buffers.
    glGenVertexArrays(1, &context.shapes_quad_vao);
    glGenBuffers(1, &context.shapes_quad_vbo);
    glGenBuffers(1, &context.shapes_quad_ebo);
    glBindVertexArray(context.shapes_quad_vao); // start recording.
    glBindBuffer(GL_ARRAY_BUFFER, context.shapes_quad_vbo); // send the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.shapes_quad_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0); //cleanup.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mat4x4_identity(context.shapes_model_matrix);

    // FONT //
    const char* font_vertex_src = 
    GLE2D_DEFAULT_SHADER_VERSION
    "layout (location = 0) in vec4 vertex;\n"
    "out vec2 TexCoords;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
    "   TexCoords = vertex.zw;\n"
    "}\n";

    const char* font_fragment_src = 
    GLE2D_DEFAULT_SHADER_VERSION
    "in vec2 TexCoords;\n"
    "out vec4 color;\n"
    "uniform sampler2D text;\n"
    "uniform vec3 text_color;\n"
    "void main()\n"
    "{\n"
    "   vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
    "   color = vec4(text_color, 1.0) * sampled;\n"
    "}\n";

    context.font_shader_program = gle2d_internal_create_shader_from_data(font_vertex_src, font_fragment_src);
    if (context.font_shader_program == 0) {
        return 1;
    }
    context.font_projection_loc = glGetUniformLocation(context.font_shader_program, "projection");
    context.font_text_color_loc = glGetUniformLocation(context.font_shader_program, "text_color");

    int max_buffer_size = GLE2D_FONT_MAX_TEXT_LEN * 6 * 4 * sizeof(float);
    // First glGen...
    glGenVertexArrays(1, &context.font_quad_vao);
    glGenBuffers(1, &context.font_quad_vbo);
    glBindVertexArray(context.font_quad_vao); // Start recording...
    glBindBuffer(GL_ARRAY_BUFFER, context.font_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, max_buffer_size, NULL, GL_DYNAMIC_DRAW); 
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // "Cleanup"...
    glBindVertexArray(0); // EBO must be unboud after VBO
                          // unbind beacouse it is "recording".

    context.font_batch_buffer = malloc(max_buffer_size);
    if (!context.font_batch_buffer) {
        return 1;
    }

    mat4x4_identity(context.projection_matrix);
    return 0;
}
// The current drawable framebuffer dimensions (the values you pass to glViewport).
void gle2d_update_rendering_area(int viewport_width, int viewport_height)
{
    mat4x4_identity(context.projection_matrix);
    mat4x4_ortho(context.projection_matrix, 0.0f, viewport_width, viewport_height, 0.0f, -1.0f, 1.0f);
    glUseProgram(context.font_shader_program);
    glUniformMatrix4fv(context.font_projection_loc, 1, GL_FALSE, &context.projection_matrix[0][0]);
    glUseProgram(context.shapes_shader_program);
    glUniformMatrix4fv(context.shapes_loc_proj, 1, GL_FALSE, &context.projection_matrix[0][0]);
}

void gle2d_shutdown(void)
{
    glUseProgram(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteProgram(context.shapes_shader_program);
    glDeleteProgram(context.font_shader_program);
    glDeleteBuffers(1, &context.font_quad_vbo);
    glDeleteBuffers(1, &context.shapes_quad_vbo);
    glDeleteBuffers(1, &context.shapes_quad_ebo);
    glDeleteVertexArrays(1, &context.font_quad_vao);
    glDeleteVertexArrays(1, &context.shapes_quad_vao);
    free(context.font_batch_buffer);
}

///////////
// Shapes

void gle2d_shapes_draw_quad(float x, float y, float w, float h, vec4 color, GLuint texture)
{       
    mat4x4_identity(context.shapes_model_matrix);
    mat4x4_translate_in_place(context.shapes_model_matrix, x, y, 1.0f);
    mat4x4_scale_aniso(context.shapes_model_matrix, context.shapes_model_matrix, w, h, 1.0f);
    //mat4x4_rotate_Z(context.shapes_model_matrix, context.shapes_model_matrix, 1.0f);

    glUseProgram(context.shapes_shader_program);
    glUniform4fv(context.shapes_loc_color, 1, color);
    glUniformMatrix4fv(context.shapes_loc_model, 1, GL_FALSE, &context.shapes_model_matrix[0][0]);
    int use_texture = texture > 0 ? 1 : 0;
    glUniform1i(context.shapes_loc_flag, use_texture);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(context.shapes_quad_vao);
    if (use_texture) {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

//////////
// FONT

int gle2d_font_create(GLE2D_Font *font, const char *path, float px_size)
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

void gle2d_font_render_text(const GLE2D_Font* font, const char* text, float x, float y)
{
    if (!font || !text || *text == '\0') {
        return;
    }

    glUseProgram(context.font_shader_program);
    glUniform3f(context.font_text_color_loc, 0.5f, 0.8f, 0.2f);

    glBindVertexArray(context.font_quad_vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->atlas);
    glBindBuffer(GL_ARRAY_BUFFER, context.font_quad_vbo);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    size_t text_len = strlen(text);
    assert(text_len <= GLE2D_FONT_MAX_TEXT_LEN);
    if (text_len > GLE2D_FONT_MAX_TEXT_LEN) {
        text_len = GLE2D_FONT_MAX_TEXT_LEN;
    } 

    float xpos = x;
    float ypos = y;
    int cursor = 0;

    const char* p = text; // walk the string without modifying 'text'
    while (*p != '\0') {                     // strlen gotcha!!
        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(font->packed_char_array,
                            GLE2D_FONT_ATLAS_SIZE,
                            GLE2D_FONT_ATLAS_SIZE,
                            *p - GLE2D_FONT_FIRST_CHAR,
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

        memcpy(&context.font_batch_buffer[cursor], char_vertices, sizeof(char_vertices));
        cursor += 6 * 4;
        p++;
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, (cursor * sizeof(float)), context.font_batch_buffer);
    glDrawArrays(GL_TRIANGLES, 0, cursor / 4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void gle2d_font_destroy(GLE2D_Font* font)
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

/////////////
// TEXTURES

GLE2D_Texture gle2d_texture_load(const char* path)
{
    GLE2D_Texture texture = {0};

    int width;
    int height;
    int channel_number;
    stbi_set_flip_vertically_on_load(1);                            // TODO ------- Questionable 4??
    unsigned char* data = stbi_load(path, &width, &height, &channel_number, 4);
    stbi_set_flip_vertically_on_load(0);
    if (!data) {
        return texture;
    } 

    glGenTextures(1, &texture.id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    texture.width = width;
    texture.height = height;
    return texture;
}

void gle2d_texture_bind(GLE2D_Texture texture)
{
    assert(texture.id != 0);
    glBindTexture(GL_TEXTURE_2D, texture.id);
}

void gle2d_texture_delete(GLE2D_Texture texture)
{
    assert(texture.id != 0);
    glDeleteTextures(1, &texture.id);
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