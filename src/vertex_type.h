#ifndef VERTEX_TYPE_H
#define VERTEX_TYPE_H

#include <glad/glad.h>

typedef struct s_vertex {
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat uv[2];
} Vertex;


#endif