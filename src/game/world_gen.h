#ifndef DG_WORLD_GEN_H
#define DG_WORLD_GEN_H

#include <stdint.h>

typedef enum {
    BLOCK_AIR = 0,
    BLOCK_STONE,
    BLOCK_TYPE_COUNT,
} block_type;

typedef struct {
    uint8_t blocks[128][16][16];
} Chunk;

void chunk_create(Chunk* chunk);

#endif