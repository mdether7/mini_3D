#ifndef GL_HELPERS_H
#define GL_HELPERS_H

#include <glad/glad.h>

GLenum gl_check_error_(const char* file, int line); // for manulal error checks!
void APIENTRY gl_debug_output_callback(GLenum source, GLenum type, unsigned int id,
    GLenum severity, GLsizei length, const char *message, const void *userParam);

#ifndef ENABLE_GL_DEBUG_OUTPUT
#define glCheckError() gl_check_error_(__FILE__, __LINE__)
#endif

#endif // GL_HELPERS_H