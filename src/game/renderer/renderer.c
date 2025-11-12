#include "renderer.h"

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#include <glad/glad.h>
#include <linmath/linmath.h>

#include "shader.h"

// Cube with minimal data for displaying textures / using normals.
static float dg3d_vertex_data[] = {
    
    // CUBE //////
    // Front face
    -.5, .5, .5, // 0, 1
    -.5, -.5, .5, // 0, 0
    .5, -.5, .5, // 1, 0
    .5, .5, .5,  // 1, 1
    // Back face
    .5, .5, -.5,
    .5, -.5, -.5,
    -.5, -.5, -.5,
    -.5, .5, -.5,
    // Right face
    .5, .5, .5,
    .5, -.5, .5,
    .5, -.5, -.5,
    .5, .5, -.5,
    // Left face
    -.5, .5, -.5,
    -.5, -.5, -.5,
    -.5, -.5, .5,
    -.5, .5, .5,
    // Top face
    -.5, .5, -.5,
    -.5, .5, .5,
    .5, .5, .5,
    .5, .5, -.5,
    // Bottom face
    -.5, -.5, .5,
    -.5, -.5, -.5,
    .5, -.5, -.5,
    .5, -.5, .5,
    //////////////
};

static unsigned int dg3d_index_data[] = {

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

};

int dg3d_renderer_init(DG3D_Renderer* renderer, int fb_width, int fb_height)
{   
    assert(renderer);
    assert(fb_width > 0 && fb_height > 0);

    // Configure data.

    // Configure framebuffer //
    glGenFramebuffers(1, &renderer->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->fbo); // bind FBO

    glGenTextures(1, &renderer->fbo_texture);
    glBindTexture(GL_TEXTURE_2D, renderer->fbo_texture); // bind texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, fb_width, fb_height, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                        GL_TEXTURE_2D, renderer->fbo_texture, 0);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

    glGenRenderbuffers(1, &renderer->fbo_renderbuffer); // bind renderbuffer
    glBindRenderbuffer(GL_RENDERBUFFER, renderer->fbo_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fb_width, fb_height); 
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderer->fbo_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); // unbind renderbuffer

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return 1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind FBO.

    // Init shaders //
    renderer->shader_default.shader = shader_program_compile_from_path("shaders/dg3d_default.vert", "shaders/dg3d_default.frag");
    if (renderer->shader_default.shader.id == 0) return 1;


    renderer->viewport_width = fb_width;
    renderer->viewport_height = fb_height;
    glViewport(0, 0, fb_width, fb_height); //? To be here or not to be here?

    return 0;
}

void dg3d_renderer_shutdown(DG3D_Renderer* renderer)
{
    if (!renderer) return;

    // Shaders Cleanup.
    shader_program_delete(&renderer->shader_default.shader);
    renderer->shader_default.u_model = -1; // -1 for shader uniforms.
    renderer->shader_default.u_res   = -1;
    renderer->shader_default.u_time  = -1;

    // Buffers cleanup.
    glDeleteVertexArrays(1, &renderer->screen_quad_vao);
    glDeleteBuffers(1, &renderer->screen_quad_vbo);

    // main FBO cleanup.
    glDeleteFramebuffers(1, &renderer->fbo);
    glDeleteRenderbuffers(1, &renderer->fbo_renderbuffer);
    glDeleteTextures(1, &renderer->fbo_texture);
    renderer->fbo = 0;
    renderer->fbo_texture = 0;
    renderer->fbo_renderbuffer = 0;

    renderer->viewport_width = 0;
    renderer->viewport_height = 0;
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




