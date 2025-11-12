#ifndef DG_WORLD_GEN_H
#define DG_WORLD_GEN_H

#include <stdint.h>

typedef enum {
    AIR = 0,
    DIRT,
    STONE,
    BLOCK_COUNT
} block_type; 

typedef struct {
    uint8_t blocks[256][16][16];
} Chunk;

void fill_chunk(Chunk* chunk, block_type type);
void print_chunk(Chunk* chunk);



#endif