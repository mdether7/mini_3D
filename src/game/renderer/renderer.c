#include "renderer.h"

#include <assert.h>

#include <glad/glad.h>
#include <linmath/linmath.h>

#include "shader.h"

#define MEBIBYTE(x) ((size_t)(x) * 1024 * 1024)

// Cube with minimal data for displaying textures / using normals.
static float dg3d_vertex_data_cube[] = {
    
    // CUBE //////
    // Front face
    -.5, .5, .5, 
    -.5, -.5, .5,
    .5, -.5, .5,
    .5, .5, .5, 
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

static unsigned int dg3d_index_data_cube[] = {

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

    // Region Renderer specific initialization
    renderer->viewport_width = fb_width;
    renderer->viewport_height = fb_height;

#pragma region Buffer and Geometry setup

    // BIGBUFCIO - will see how it goes.
    glGenBuffers(1, &renderer->BIG_BUFCIO);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->BIG_BUFCIO);
    glBufferData(GL_ARRAY_BUFFER, MEBIBYTE(1), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // GLuint quad_VAO;
    // glGenVertexArrays(1, &quad_VAO);
    // glBindVertexArray(quad_VAO);

    // GLuint quad_VBO;
    // glGenBuffers(1, &quad_VBO);
    // glBindBuffer(GL_ARRAY_BUFFER, quad_VBO);
    // glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(geo_quad_vertices), geo_quad_vertices, GL_STATIC_DRAW);
    
    // glEnableVertexAttribArray(0); // only enable position.
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);


#pragma endregion

#pragma region FBO
    // Configure framebuffer.
    glGenFramebuffers(1, &renderer->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, renderer->FBO); // bind FBO

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
#pragma endregion

    // init shaders.
    renderer->shader_default.shader = shader_program_compile_from_path("shaders/dg3d_default.vert", "shaders/dg3d_default.frag");
    if (renderer->shader_default.shader.id == 0) return 1;

    glViewport(0, 0, fb_width, fb_height);
    return 0;
}

void dg3d_renderer_shutdown(DG3D_Renderer* renderer)
{
    // Shaders Cleanup
    shader_program_delete(&renderer->shader_default.shader);
    renderer->shader_default.u_model = -1; // -1 for shader uniforms.
    renderer->shader_default.u_res   = -1;
    renderer->shader_default.u_time  = -1;

    // main FBO cleanup.
    glDeleteFramebuffers(1, &renderer->FBO);
    glDeleteRenderbuffers(1, &renderer->fbo_renderbuffer);
    glDeleteTextures(1, &renderer->fbo_texture);
    renderer->FBO = 0;
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




