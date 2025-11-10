#ifndef DG_RENDERER_H
#define DG_RENDERER_H

#include "shader.h"

// I think i can forward declare camera (TODO: check it!)
// as of right now it's just a pointer to it.

// typedef struct {
//     GLuint        VAO;
//     GLuint        VBO;
//     GLuint        EBO;
// } DG3D_Mesh; //? Maybe?

typedef struct {
    DG3D_Shader shader;
    //? NOTE: Maybe add source remembering or someshit?? 
    GLint u_model;
    GLint u_time;
    GLint u_res;
} DG3D_DefaultShader; 

typedef struct {

    DG3D_DefaultShader shader_default;
    //DG3D_Camera*       camera;

    GLuint BIG_BUFCIO;

    GLuint FBO;
    GLuint fbo_texture;      // RGBA8           
    GLuint fbo_renderbuffer; // DEPTH 24 STENCIL 8

    int viewport_width;
    int viewport_height;
    
} DG3D_Renderer;

int dg3d_renderer_init(DG3D_Renderer* renderer, int width, int height);
void dg3d_renderer_shutdown(DG3D_Renderer* renderer);



#endif