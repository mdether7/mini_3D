#include "world_gen.h"
#include <stdio.h>


void fill_chunk(Chunk* chunk, block_type type)
{
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                chunk->blocks[i][j][k] = type;
            } 
        }
    }
}

void print_chunk(Chunk* chunk)
{
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                printf("%d", chunk->blocks[i][j][k]);
            } 
        }
    }
}