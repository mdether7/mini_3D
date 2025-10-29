#ifndef PLATFORM_DEBUG_H
#define PLATFORM_DEBUG_H

#include <glad/glad.h>

void debug_gl_enable(void);
void debug_display_information(void);
GLenum gl_check_error_(const char* file, int line);
void APIENTRY gl_debug_output_callback(GLenum source, GLenum type, unsigned int id,
    GLenum severity, GLsizei length, const char *message, const void *userParam);

#endif