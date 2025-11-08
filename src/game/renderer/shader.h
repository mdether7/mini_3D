#ifndef DG_SHADER_H
#define DG_SHADER_H

#include <glad/glad.h>

typedef struct {
    GLuint id;
} DG3D_Shader;

DG3D_Shader shader_program_compile_from_path(const char* vert_path, const char* frag_path);
DG3D_Shader shader_program_tess_compile_from_path(const char* vert_path, 
    const char* tcs_path, const char* tes_path, const char* frag_path);
void   shader_program_delete(DG3D_Shader* shader);
void   shader_program_bind(DG3D_Shader* shader);

#endif