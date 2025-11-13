#include "world_gen.h"

#include <string.h>
#include <stdlib.h>
#include <time.h>

// Generate random integer in range [min, max] (inclusive)
static inline int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Generate random float in range [0.0, 1.0]
static inline float random_float(void) {
    return (float)rand() / (float)RAND_MAX;
}

// Generate random float in range [min, max]
static inline float random_float_range(float min, float max) {
    return min + random_float() * (max - min);
}

void chunk_create(Chunk* chunk)
{
    memset(chunk->blocks, BLOCK_AIR, sizeof(chunk->blocks));

    for (int y = 0; y < 64; y++) {
        for (int z = 0; z < 16; z++) {
            for (int x = 0; x < 16; x++) {
                chunk->blocks[y][z][x] = BLOCK_STONE;
            }
        }
    }
}