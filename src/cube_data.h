#ifndef CUBE_DATA_H
#define CUBE_DATA_H

#include "glad/glad.h"

#include "vertex_type.h"

/**
 * Texture coordinates UVs
 * (0,1)    (1,1)      U = x-axis
 *                     V = y-axis
 * (0,0)    (1,0)
 */

static const Vertex cube_vertices[] = {
//   Position               Normal              UV
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f,}}, // front face
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f,}},
    {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f,}},
    {{1.0f, -1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f,}},

    {{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f,}},
    {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f,}},
    
};

static const GLuint cube_indices[] = {
    0, 1, 2,  2, 3, 0,   // front
    4, 5, 6,  6, 7, 4,   // back
    0, 4, 7,  7, 3, 0,   // left
    1, 5, 6,  6, 2, 1,   // right
    3, 2, 6,  6, 7, 3,   // top
    0, 1, 5,  5, 4, 0    // bottom
};

#endif