#ifndef DUNGEN_H
#define DUNGEN_H

#include <stddef.h>

#define DUN_SIZE 32 /* Whole dungeon is 32 x 32 = 1024 tiles */
#define DUN_ROOM_SIZE 8 /* Each "room cell" is 8 X 8 = 16 room tiles */
#define DUN_TOTAL_CELLS ((DUN_SIZE / DUN_ROOM_SIZE) * (DUN_SIZE / DUN_ROOM_SIZE))

#define MIN_ROOM_COUNT 4
#define MAX_ROOM_COUNT 8

struct room {
    enum tile_type layout[DUN_ROOM_SIZE][DUN_ROOM_SIZE];
};

enum tile_type {
    FLOOR = 0, WALL, DOOR
};

enum room_type {
    ROOM_START = 0, ROOM_CAVE, ROOM_CROSS, ROOM_RECT, ROOM_SQUARE, ROOM_TOTAL
};

extern enum tile_type maptiles[DUN_SIZE][DUN_SIZE];

int dungeon_generate(void);

#endif