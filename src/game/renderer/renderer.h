#ifndef DG_RENDERER_H
#define DG_RENDERER_H

#include <stddef.h>
#include <linmath/linmath.h>
#include "../../../gleter2d/gleter2d.h"
#include "shader.h"

typedef struct DG3D_Camera DG3D_Camera;

///////////////////
// Unifrom Buffer
typedef struct {
    GLuint handle;
    size_t size;
    GLuint binding_point;
} DG3D_UniformBuffer;

int dg3d_uniform_buffer_create(DG3D_UniformBuffer* ubo, size_t size, GLuint binding_point, GLenum usage);
void dg3d_uniform_buffer_destroy(DG3D_UniformBuffer* ubo);

////////////
// Shaders
typedef struct {
    GLuint      id;
    GLint       u_model;
} DefaultShader;

typedef struct {
    GLuint id;
} ScreenQuadShader;

/////////////
// Renderer
typedef struct {

    mat4x4 view;
    mat4x4 projection;

    DefaultShader    shader_default;
    ScreenQuadShader shader_screen_quad;
    DG3D_Camera     *current_camera;

    DG3D_UniformBuffer ubo_matrices;

    GLuint cube_vao;
    GLuint cube_vbo;
    GLuint cube_ebo;

    GLuint screen_quad_vao;
    GLuint screen_quad_vbo;

    GLuint fbo_main;
    GLuint fbo_main_texture;      // RGBA8           
    GLuint fbo_main_renderbuffer; // DEPTH 24 STENCIL 8

    int viewport_width;
    int viewport_height;
    
} DG3D_Renderer;

int  dg3d_renderer_init(DG3D_Renderer* renderer, int width, int height);
void dg3d_renderer_shutdown(DG3D_Renderer* renderer);

#endif