#ifndef MINI_DUNMESH_H
#define MINI_DUNMESH_H

#include <stddef.h>

#include "dungen/dungen.h"
#include "vertex_type.h"

/**
 *  The only thing you need for a mesh is a set
 *  of points, and how they connect to form faces
 *  (usually we form quads or triangles for ease of representation).
 */

/**
 * TODO: Read on greedy meshing.
 * Right now it's brute force approach XD.
 */

typedef struct {
    Vertex3D*     vertices;
    unsigned int* indices;
    size_t        vert_count;
    size_t        indices_count;
} DungeonMesh;

DungeonMesh* dungeon_generate_mesh(tile_type dungeon[DUN_SIZE][DUN_SIZE]);
void         dungeon_free_mesh(DungeonMesh* dungeon);

#endif