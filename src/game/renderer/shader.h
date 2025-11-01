#ifndef DG_SHADER_H
#define DG_SHADER_H

#include <glad/glad.h>

typedef struct {
    GLuint id;
} Shader;

int shader_program_compile_from_path(const char* vert_path, const char* frag_path);



#endif