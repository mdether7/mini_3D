#ifndef DG_RENDERER_H
#define DG_RENDERER_H

#include <stddef.h>
#include <linmath/linmath.h>
#include "../../../gleter2d/gleter2d.h"
#include "shader.h"

typedef struct DG3D_Camera DG3D_Camera;

typedef struct {
    DG3D_Shader shader;
    //? NOTE: Maybe add source remembering or someshit?? 
    GLint u_model;
    GLint u_time;
    GLint u_res;
} DefaultShader;

typedef struct {
    DG3D_Shader shader;
} ScreenQuadShader;

typedef struct {

    DefaultShader    shader_default;
    ScreenQuadShader shader_screen_quad;
    DG3D_Camera     *camera;

    GLuint cube_vao;
    GLuint cube_vbo;
    GLuint cube_ebo;

    GLuint screen_quad_vao;
    GLuint screen_quad_vbo;

    GLuint fbo;
    GLuint fbo_texture;      // RGBA8           
    GLuint fbo_renderbuffer; // DEPTH 24 STENCIL 8

    int viewport_width;
    int viewport_height;
    
} DG3D_Renderer;

int  dg3d_renderer_init(DG3D_Renderer* renderer, int width, int height);
void dg3d_renderer_shutdown(DG3D_Renderer* renderer);
void dg3d_renderer_draw_cube(DG3D_Renderer* renderer, mat4x4 model);

#endif