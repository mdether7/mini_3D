#include "renderer.h"

#include <glad/glad.h>
#include <linmath/linmath.h>

#include "shader.h"

// Cube with minimal data for displaying textures / using normals.
static float dg3d_renderer_vertex_data_cube[] = {
    
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

static unsigned int dg3d_renderer_index_data_cube[] = {

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


// uv
// 0,1,
// 0,0,
// 1,0,
// 1,1

// void glDrawArrays( 	GLenum mode,
//   	GLint first,
//   	GLsizei count);
// first specifies per vertex offset (not byte offset) (LIVES IN VBO)

// void glDrawElements( 	GLenum mode,
//   	GLsizei count,
//   	GLenum type,
//   	const void * indices);
// indices specifies index per byte offset (LIVES IN EBO)


typedef struct {
    GLuint        VAO;
    GLuint        VBO;
    GLuint        EBO;
} DG3D_Mesh;

typedef struct {
    DG3D_Shader shader;
    //? NOTE: Maybe add source remembering or someshit?? 
    GLint u_model;
    GLint u_time;
    GLint u_res;
} DG3D_DefaultShader; 

typedef struct {

    DG3D_DefaultShader shader_default;

    GLuint FBO;

    int viewport_width;
    int viewport_height;
    
} DG3D_Renderer;

int dg3d_renderer_init(DG3D_Renderer* renderer)
{   
    glGenFramebuffers(1, &renderer->FBO);

    // init shaders.
    renderer->shader_default.shader = shader_program_compile_from_path("shaders/dg3d_default.vert", "shaders/dg3d_default.frag");
    if (renderer->shader_default.shader.id == 0) return 1;



}

void dg3d_renderer_shutdown(DG3D_Renderer* renderer)
{
    // Shaders Cleanup
    shader_program_delete(&renderer->shader_default.shader);

    // Buffer Cleanup
    glDeleteFramebuffers(1, &renderer->FBO);
}

void dg3d_render_mesh(DG3D_Mesh* mesh, vec3 pos)
{

}


