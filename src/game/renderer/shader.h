#ifndef DG_SHADER_H
#define DG_SHADER_H

#include <glad/glad.h>

typedef struct {
    GLuint id;
} Shader;

Shader shader_program_compile_from_path(const char* vert_path, const char* frag_path);
Shader shader_program_tess_compile_from_path(const char* vert_path, 
    const char* tcs_path, const char* tes_path, const char* frag_path);
void   shader_program_delete(Shader* shader);
void   shader_program_bind(Shader* shader);

#endif