#ifndef VERTEX_TYPE_H
#define VERTEX_TYPE_H

#include <glad/glad.h>

typedef struct s_vertex_3d {
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat uv[2];
} Vertex3D;

typedef struct s_vertex_2d {
    GLfloat position[3]; // Z for depth.
    GLfloat uv[2];
} Vertex2D;

#endif