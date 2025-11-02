#include "platform_debug.h"

#include <glad/glad.h>
#include <stdio.h>

#include "platform_log.h"

void debug_gl_enable(void)
{
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_output_callback, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                GL_DONT_CARE, 0, NULL, GL_TRUE);
        platform_log_info("[GL] Debug output enabled!");
    }
    else
    {
        platform_log_error("[GL] Debug output could not be enabled!");
    }
} 

void debug_display_information(void)
{
    GLint i1, i2, i3;
    GLint dims[2];
    GLint max_size;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &i1);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &i2);
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &i3);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, dims);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);

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
    fflush(stdout);
}

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
        platform_log_error("%s | %s (%d)\n", error, file, line);
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
    platform_log_error("[GL DEBUG] Debug message (ID: %u) -> %s | %s | %s | %s\n", id, message, _source, _type, _severity);
}