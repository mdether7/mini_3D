#include "renderer.h"

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include <glad/glad.h>
#include <linmath/linmath.h>

#include "math_misc.h"
#include "shader.h"

#define U_BLOCK_MATRICES_BINDING 1 // 0 is for gleter2d

// Cube with minimal data for displaying textures / using normals.
static float dg3d_vertex_data[] = {
    
    // CUBE //////
    // Front face  TexCoords
    -.5, .5, .5,   0.0, 1.0,
    -.5, -.5, .5,  0.0, 0.0,
    .5, -.5, .5,   1.0, 0.0,
    .5, .5, .5,    1.0, 1.0,
    // Back face
    .5, .5, -.5,   0.0, 1.0,
    .5, -.5, -.5,  0.0, 0.0,
    -.5, -.5, -.5, 1.0, 0.0,
    -.5, .5, -.5,  1.0, 1.0,
    // Right face 
    .5, .5, .5,    0.0, 1.0,       
    .5, -.5, .5,   0.0, 0.0,      
    .5, -.5, -.5,  1.0, 0.0,     
    .5, .5, -.5,   1.0, 1.0,       
    // Left face
    -.5, .5, -.5,  0.0, 1.0,    
    -.5, -.5, -.5, 0.0, 0.0,      
    -.5, -.5, .5,  1.0, 0.0,     
    -.5, .5, .5,   1.0, 1.0,     
    // Top face
    -.5, .5, -.5,  0.0, 1.0,
    -.5, .5, .5,   0.0, 0.0,
    .5, .5, .5,    1.0, 0.0,
    .5, .5, -.5,   1.0, 1.0,
    // Bottom face
    -.5, -.5, .5,  0.0, 1.0,
    -.5, -.5, -.5, 0.0, 0.0,
    .5, -.5, -.5,  1.0, 0.0,
    .5, -.5, .5,   1.0, 1.0,
    //////////////
};

static unsigned short dg3d_index_data[] = {

    // CUBE //////
    // Front face
    0, 1, 2,
    2, 3, 0,
    // Back face
    4, 5, 6,
    6, 7, 4,
    // Right face
    8, 9, 10,
    10, 11, 8,
    // Left face
    12, 13, 14,
    14, 15, 12,
    // Top face
    16, 17, 18,
    18, 19, 16,
    // Bottom face
    20, 21, 22,
    22, 23, 20,
    ////////////// 
};

static float dg3d_fullscreen_quad[] = {
  // Position // TexCoord (UV/ST)
   -1.,  1.,     .0,   1.,
   -1., -1.,     .0,  .0,
    1., -1.,      1., .0,

    1., -1.,      1., .0,
    1.,  1.,      1.,  1.,
   -1.,  1.,     .0,   1.
  //////////////////////////////
};

/////////////
// Renderer

int dg3d_renderer_init(DG3D_Renderer* renderer, int fb_width, int fb_height)
{   
    assert(renderer);
    assert(fb_width > 0 && fb_height > 0);

    // Init shaders 
    renderer->shader_default.id = shader_program_compile_from_path("shaders/dg3d_default.vert", "shaders/dg3d_default.frag");
    renderer->shader_screen_quad.id = shader_program_compile_from_path("shaders/dg3d_default_screen.vert", "shaders/dg3d_default_screen.frag");

    if (renderer->shader_default.id == 0 ||renderer->shader_screen_quad.id == 0) {
        shader_program_delete(renderer->shader_default.id);
        shader_program_delete(renderer->shader_screen_quad.id);
        return 1;
    }

    renderer->shader_default.u_model = shader_get_uniform_location(renderer->shader_default.id, "uModel");

    GLuint u_block_index = shader_get_uniform_block_index(renderer->shader_default.id, "uBlockMatrices");
    glUniformBlockBinding(renderer->shader_default.id, u_block_index, U_BLOCK_MATRICES_BINDING);

    dg3d_uniform_buffer_create(&renderer->ubo_matrices, 2 * sizeof(mat4x4), U_BLOCK_MATRICES_BINDING, GL_STREAM_DRAW);

    // Screen Quad VAO
    glGenVertexArrays(1, &renderer->screen_quad_vao);
    glGenBuffers(1, &renderer->screen_quad_vbo);
    glBindVertexArray(renderer->screen_quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->screen_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dg3d_fullscreen_quad), dg3d_fullscreen_quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, (sizeof(float) * 4), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Cube VAO
    glGenVertexArrays(1, &renderer->cube_vao);
    glGenBuffers(1, &renderer->cube_vbo);
    glGenBuffers(1, &renderer->cube_ebo);
    glBindVertexArray(renderer->cube_vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->cube_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->cube_ebo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dg3d_vertex_data), dg3d_vertex_data, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(dg3d_index_data), dg3d_index_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 5), (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(0);             // This is the full stride  // This is offset in a stride
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Configure framebuffer //
    glGenFramebuffers(1, &renderer->fbo_main);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->fbo_main); // bind FBO

    glGenTextures(1, &renderer->fbo_main_texture);
    glBindTexture(GL_TEXTURE_2D, renderer->fbo_main_texture); // bind texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, fb_width, fb_height, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                        GL_TEXTURE_2D, renderer->fbo_main_texture, 0);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

    glGenRenderbuffers(1, &renderer->fbo_main_renderbuffer); // bind renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, renderer->fbo_main_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fb_width, fb_height); 
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderer->fbo_main_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); // unbind renderbuffer

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return 1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind FBO.

    renderer->viewport_width = fb_width;
    renderer->viewport_height = fb_height;
    renderer->camera_current = NULL;
    glViewport(0, 0, fb_width, fb_height); //? To be here or not to be here?

    return 0;
}

void dg3d_renderer_shutdown(DG3D_Renderer* renderer)
{
    if (!renderer) return;

    // Shaders Cleanup.
    shader_program_delete(renderer->shader_default.id);
    shader_program_delete(renderer->shader_screen_quad.id);

    // Ubo
    dg3d_uniform_buffer_destroy(&renderer->ubo_matrices);

    // Screen quad cleanup.
    glDeleteVertexArrays(1, &renderer->screen_quad_vao);
    glDeleteBuffers(1, &renderer->screen_quad_vbo);

    // Cube cleanup.
    glDeleteVertexArrays(1, &renderer->cube_vao);
    glDeleteBuffers(1, &renderer->cube_vbo);
    glDeleteBuffers(1, &renderer->cube_ebo);

    // main FBO cleanup.
    glDeleteFramebuffers(1, &renderer->fbo_main);
    glDeleteRenderbuffers(1, &renderer->fbo_main_renderbuffer);
    glDeleteTextures(1, &renderer->fbo_main);
}

///////////////////
// Uniform Buffer
int dg3d_uniform_buffer_create(DG3D_UniformBuffer* ubo, size_t size, GLuint binding_point, GLenum usage)
{
    glGenBuffers(1, &ubo->handle);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo->handle);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, usage);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, ubo->handle);

    ubo->binding_point = binding_point;
    ubo->size = size;

    return 0;
}

void dg3d_uniform_buffer_destroy(DG3D_UniformBuffer* ubo)
{
    if (!ubo) return;
    glDeleteBuffers(1, &ubo->handle);
}


// void glDrawArrays( 	GLenum mode,
//   	GLint first,
//   	GLsizei count);
// first specifies per vertex offset (not byte offset) (LIVES IN VBO)

// void glDrawElements( 	GLenum mode,
//   	GLsizei count,
//   	GLenum type,
//   	const void * indices);
// indices specifies index per byte offset (LIVES IN EBO)




