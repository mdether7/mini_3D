#ifndef DG_RENDERER_H
#define DG_RENDERER_H

#include <stddef.h>
#include "shader.h"

// I think i can forward declare camera (TODO: check it!)
// as of right now it's just a pointer to it.

typedef struct {
    DG3D_Shader shader;
    //? NOTE: Maybe add source remembering or someshit?? 
    GLint u_model;
    GLint u_time;
    GLint u_res;
} DG3D_DefaultShader; 

typedef struct {
    size_t vertex_offset;
    size_t index_offset;
    size_t vertex_count;
    size_t index_count;
} DG3D_Mesh;

typedef struct {

    DG3D_DefaultShader shader_default;
    //DG3D_Camera*       camera;

    DG3D_Mesh* meshes;
    size_t     mesh_count;

    GLuint BIG_BUFCIO_VAO;
    GLuint BIG_BUFCIO_VBO;
    GLuint BIG_BUFCIO_EBO;

    GLuint SCREEN_QUAD_VAO;
    GLuint SCREEN_QUAD_VBO;

    GLuint FBO;
    GLuint fbo_texture;      // RGBA8           
    GLuint fbo_renderbuffer; // DEPTH 24 STENCIL 8

    int viewport_width;
    int viewport_height;
    
} DG3D_Renderer;

int dg3d_renderer_init(DG3D_Renderer* renderer, int width, int height);
void dg3d_renderer_shutdown(DG3D_Renderer* renderer);

#endif