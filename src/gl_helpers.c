#include "gl_helpers.h"

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <glad/glad.h>

#include "file_read.h"
#include "mini_utils.h"

//////////////////////////////////////////////
// OpenGL debugging (Thanks learnopengl.com)

GLenum
gl_check_error_(const char* file, int line)
{
    GLenum error_enum;
    while ((error_enum = glGetError()) != GL_NO_ERROR)
    {
        const char* error;
        switch (error_enum)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            default: error = "UNKNOWN ERROR"; break;
        }
        fprintf(stderr, "%s | %s (%d)\n", error, file, line);
    }
    return error_enum;
}

void APIENTRY gl_debug_output_callback(GLenum source, GLenum type, unsigned int id,
    GLenum severity, GLsizei length, const char *message, const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    char* _source;
    char* _type;
    char* _severity;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             _source = "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   _source = "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: _source = "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     _source = "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     _source = "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           _source = "Source: Other"; break;
        default: _source = "Source: Unknown(default switch case)"; break;
    } 

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               _type = "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: _type = "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  _type = "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         _type = "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         _type = "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              _type = "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          _type = "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           _type = "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               _type = "Type: Other"; break;
        default: _type = "Type: Unknown [Default switch case!]"; break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         _severity = "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       _severity = "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          _severity = "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: _severity = "Severity: notification"; break;
        default: _severity = "Severity: Unknown [Default switch case!]"; break;
    }

    fprintf(stdout, TERMINAL_RED "Debug message (ID: %u) -> %s | %s | %s | %s\n" TERMINAL_RESET, 
                    id, message, _source, _type, _severity);
    // just to be sure
    fflush(stdout);
}

////////////////
// OpenGL misc
void misc_gl_display_information(void)
{
    GLint i1, i2, i3;
    GLint dims[2];
    GLint max_size;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &i1);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &i2);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &i3);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, dims);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);

    fprintf(stdout, TERMINAL_GREEN);
    fprintf(stdout, "[OPENGL INFORMATION]\n");
    fprintf(stdout, 
        "OpenGL Version: %s\n"
        "GLSL Version  : %s\n"
        "Renderer      : %s\n"
        "Vendor        : %s\n", 
        glGetString(GL_VERSION),
        glGetString(GL_SHADING_LANGUAGE_VERSION),
        glGetString(GL_RENDERER),
        glGetString(GL_VENDOR));
    fprintf(stdout, 
        "MAX_TEXTURE_IMAGE_UNITS: %d\n"
        "MAX_COMBINED_TEXTURE_IMAGE_UNITS: %d\n" 
        "MAX_VERTEX_TEXTURE_IMAGE_UNITS: %d\n"
        "MAX_VIEWPORT_DIMS: W: %d, H: %d\n"
        "MAX_TEXTURE_SIZE: [%dx%d]\n", i1, i2, i3, dims[0], dims[1], max_size, max_size);
    fprintf(stdout, TERMINAL_RESET);
    fflush(stdout);
}

///////////////////
// Shader Loading

static int 
shader_compile_error(GLuint shader)
{
	GLint success;
	char info_log[1024] = {0};

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
		fprintf(stderr, "SHADER ERROR! Log: %s\n", info_log);
		return -1;
	}

	return 0;
}

static int 
shader_program_link_error(GLuint program)
{
    GLint success;
    char info_log[1024] = {0};

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
        fprintf(stderr, "SHADER ERROR! Log: %s\n", info_log);
        return -1;
    }

    return 0;
}

GLuint
shader_program_compile(const char* vert_path, const char* frag_path)
{  
    assert(vert_path && frag_path);

    char*         vertex_source   = NULL;
    char*         fragment_source = NULL;

    vertex_source   = read_file(vert_path);
    fragment_source = read_file(frag_path);

    if (!vertex_source || !fragment_source) {
        free(vertex_source);
        free(fragment_source);
        return 0; // 0 never valid object in GL
    }

    GLuint vertex_shader_id   = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader_id, 1, (const GLchar**)&vertex_source, NULL);
    glShaderSource(fragment_shader_id, 1, (const GLchar**)&fragment_source, NULL);

    free(vertex_source);
    free(fragment_source);
    vertex_source   = NULL;
    fragment_source = NULL;

    glCompileShader(vertex_shader_id);
    glCompileShader(fragment_shader_id);

    if (shader_compile_error(vertex_shader_id)   < 0 ||
        shader_compile_error(fragment_shader_id) < 0) {

        glDeleteShader(vertex_shader_id);  
        glDeleteShader(fragment_shader_id);

        return 0;
    }

    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    // glDetachShader(program_id, vertex_shader_id); some legacy stuff.
    // glDetachShader(program_id, fragment_shader_id);
    glDeleteShader(vertex_shader_id);  
    glDeleteShader(fragment_shader_id);

    if (shader_program_link_error(program_id) < 0) {

        glDeleteProgram(program_id);
        return 0;

    }

    return program_id;
}

bool
shader_program_hot_reload(GLuint* program, 
    const char* vert_path, const char* frag_path)
{
    assert(program && vert_path && frag_path);

    GLuint reloaded_program = shader_program_compile(vert_path, frag_path);
    if (reloaded_program == 0)
        return false;

    glDeleteProgram(*program);
    *program = reloaded_program;

    return true;
}