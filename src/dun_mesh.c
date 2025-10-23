#include "dun_mesh.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "dungen/dungen.h"
#include "data_geometry.h"
#include "vertex_type.h"

//################################
//#########......#################
//#########......##########......#
//#########......##########......#

DungeonMesh* dungeon_generate_mesh(tile_type dungeon[DUN_SIZE][DUN_SIZE])
{
    DungeonMesh* mesh = malloc(sizeof(*mesh));
    if (mesh == NULL)
        return NULL;

    mesh->vertices      = NULL;
    mesh->indices       = NULL;
    mesh->vert_count    = 0;
    mesh->indices_count = 0;

    int non_walls = 0;
    for (int row = 0; row < DUN_SIZE; row++) {
        for (int col = 0; col < DUN_SIZE; col++) {

            if (dungeon[row][col] == WALL)
                continue;

            non_walls++;
        }
    }

    // I know it's really inefficient, but... 
    // "Make it work, make it right yadda yadda."
    mesh->vert_count    = non_walls * 4; // 4 Vertices per tile.
    mesh->indices_count = non_walls * 6; // 6 Indices per tile.

    mesh->vertices = malloc(sizeof(Vertex3D) * mesh->vert_count);
    mesh->indices  = malloc(sizeof(GLuint) * mesh->indices_count); 
    
    if (!mesh->vertices || !mesh->indices) {
        free(mesh->vertices);
        free(mesh->indices);
        free(mesh);

        return NULL;
    }

    float pos[3];
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 1.0f;

    int count_vert    = 0;
    int count_indices = 0;
    for (int row = 0; row < DUN_SIZE; row++) {
        for (int col = 0; col < DUN_SIZE; col++) {

            if (dungeon[row][col] == WALL) {
                // if wall just skip to
                pos[0] += 1.0f; // the next position.

            }
            else 
            {                                                                       // technically i dont need to store  
                                                                                    // normals for each vertex cus i know it's a floor.
                mesh->vertices[count_vert]   = (Vertex3D){{pos[0], pos[1], pos[2]},           {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}};
                mesh->vertices[count_vert+1] = (Vertex3D){{pos[0], pos[1], pos[2]-1.0f},      {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}};
                mesh->vertices[count_vert+2] = (Vertex3D){{pos[0]+1.0f, pos[1], pos[2]-1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}};
                mesh->vertices[count_vert+3] = (Vertex3D){{pos[0]+1.0f, pos[1], pos[2]},      {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}};

                // 0, 0, 0    1, 0, 0    | 0, 3, 1
                //                       | 2, 1, 3
                // 0, 0, 1    1, 0, 1

                mesh->indices[count_indices]   = count_vert;
                mesh->indices[count_indices+1] = count_vert + 3;
                mesh->indices[count_indices+2] = count_vert + 1;
                mesh->indices[count_indices+3] = count_vert + 2;
                mesh->indices[count_indices+4] = count_vert + 1;
                mesh->indices[count_indices+5] = count_vert + 3;

                pos[0]        += 1.0f; // jump one to right on X.
                count_vert    += 4;    // tile tracking
                count_indices += 6;
                
            }
        }
        pos[0] = 0.0f;  // reset in X
        pos[2] += 1.0f; // jump in Z
    }

    return mesh;
}

void dungeon_free_mesh(DungeonMesh* dungeon)
{
    if (dungeon) {
        free(dungeon->vertices);
        free(dungeon->indices);
        free(dungeon);
    }
}
