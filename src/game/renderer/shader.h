#ifndef DG_SHADER_H
#define DG_SHADER_H

#include <glad/glad.h>

GLuint shader_program_compile_from_path(const char* vert_path, const char* frag_path);
GLuint shader_program_tess_compile_from_path(const char* vert_path, 
    const char* tcs_path, const char* tes_path, const char* frag_path);
GLuint shader_get_uniform_location(GLuint id, const char* name);
GLuint shader_get_uniform_block_index(GLuint id, const char* name);
void   shader_program_delete(GLuint id);
void   shader_program_bind(GLuint id);

#endif