#include "renderer.h"

#include <glad/glad.h>
#include <linmath/linmath.h>

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

    // CUBE //
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
};


// uv
// 0,1,
// 0,0,
// 1,0,
// 1,1

typedef struct {
    float*        vertices;
    unsigned int* indices;
} DG3D_Model;

typedef struct {
    GLuint id_gl;
    //? NOTE: Maybe add source remembering or someshit?? 
    // locations as needed...
    GLint u_model;
    GLint u_time;
    GLint u_res;
} DG3D_DeafultShader; 

typedef struct {
    DG3D_DeafultShader shader_default;
    DG3D_Model cube;
} DG3D_Renderer;

DG3D_Renderer renderer;

int dg3d_renderer_init(void)
{   



}

void dg3d_renderer_shutdown(void)
{

}


