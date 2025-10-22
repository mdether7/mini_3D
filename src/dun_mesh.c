#include "dun_mesh.h"

#include <stddef.h>

#include "vertex_type.h"

DungeonMesh* generate_dungeon_floor(void)
{
    DungeonMesh* mesh   = NULL;
    mesh->vertices      = NULL;
    mesh->indices       = NULL;
    mesh->vert_count    = 0;
    mesh->indices_count = 0;

    // for (size_t row = 0; row < DUN_SIZE; row++) {
    //     for (size_t col = 0; col < DUN_SIZE; col++) {
    //         dungeon[]
    //     }
    // }
}

// TODO