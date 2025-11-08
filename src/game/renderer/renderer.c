#include "renderer.h"

#include <glad/glad.h>
#include <linmath/linmath.h>

float dg3d_vertex_data_cube[] = {

    // Front face
    -0.5,  0.5, -0.5,
    -0.5, -0.5, -0.5,
    0.5, -0.5, -0.5,
    0.5,  0.5, -0.5 
    // Back face

    

    // Right face

    // Left face

    // Top face

    // Bottom face


};

// Minimal data needed to display a cube, front face.
//vertices
// -0.5,  0.5, -0.5,
// -0.5, -0.5, -0.5,
//  0.5, -0.5, -0.5,
//  0.5,  0.5, -0.5

// indices
// 0, 1, 2,
// 2, 3, 0

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

    // locations as needed...
    GLint u_model;
    GLint u_time;
    GLint u_res;


} DG3D_DeafultShader; 

typedef struct {

    DG3D_DeafultShader shader_default;
    
} DG3D_Renderer;

DG3D_Renderer renderer;

int dg3d_renderer_init(void)
{   



}

void dg3d_renderer_shutdown(void)
{

}


