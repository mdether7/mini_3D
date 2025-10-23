#include "dun_mesh.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "dungen/dungen.h"
#include "data_geometry.h"
#include "vertex_type.h"

/**..........
   ..........
   ..........
   ..........
   ..........
   ..........
   ..........
   ..........
   ..........
   ..........
 */

DungeonMesh* dungeon_test_mesh(tile_type dun_test[10][10])
{
    DungeonMesh* mesh = malloc(sizeof(*mesh));
    if (mesh == NULL)
        return NULL;

    mesh->vertices      = NULL;
    mesh->indices       = NULL;
    mesh->vert_count    = 0;
    mesh->indices_count = 0;

    int total_tiles     = 10 * 10;
    mesh->vert_count    = total_tiles * 4; // 4 Vertices per tile.
    mesh->indices_count = total_tiles * 6; // 6 Indices per tile.

    mesh->vertices = malloc(sizeof(Vertex3D) * mesh->vert_count);
    mesh->indices  = malloc(sizeof(GLuint) * mesh->indices_count);

    if (!mesh->vertices || !mesh->indices) {
        free(mesh->indices);
        free(mesh->vertices);
        free(mesh);
        return NULL;
    }


    int count = 0;
    int tile = 0;
    float pos[3];

    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 1.0f;

    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            mesh->vertices[tile]   = (Vertex3D){{pos[0], pos[1], pos[2]}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}};
            mesh->vertices[tile+1] = (Vertex3D){{pos[0], pos[1], pos[2]-1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}};
            mesh->vertices[tile+2] = (Vertex3D){{pos[0]+1.0f, pos[1], pos[2]-1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}};
            mesh->vertices[tile+3] = (Vertex3D){{pos[0]+1.0f, pos[1], pos[2]}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}};

            pos[0] += 1.0f; // jump one to right on X.
            tile+=4;
        }
        pos[0] = 0.0f; // reset back X to 0.
        pos[2] += 1.0f; // jump "down".
    }

    return mesh;
}

DungeonMesh* dungeon_generate_mesh(tile_type dungeon[DUN_SIZE][DUN_SIZE])
{
    DungeonMesh* mesh = malloc(sizeof(*mesh));
    if (mesh == NULL)
        return NULL;

    mesh->vertices      = NULL;
    mesh->indices       = NULL;
    mesh->vert_count    = 0;
    mesh->indices_count = 0;


    for (int row = 0; row < DUN_SIZE; row++) {
        for (int col = 0; col < DUN_SIZE; col++) {
            if (dungeon[row][col] == FLOOR || dungeon[row][col] == DOOR)
                mesh->vert_count++;
        }
    }

    return mesh;
}



// TODO