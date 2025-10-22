#ifndef MINI_DUNGEN_H
#define MINI_DUNGEN_H

#include <stddef.h>

#include "vertex_type.h"

/**
 *  The only thing you need for a mesh is a set
 *  of points, and how they connect to form faces
 *  (usually we form quads or triangles for ease of representation).
 */

#define DUN_SIZE 32 // 32x32

typedef struct {
    Vertex3D*     vertices;
    unsigned int* indices;
    size_t        vert_count;
    size_t        indices_count;
} DungeonMesh;

DungeonMesh* generate_dungeon_floor(void);

#endif