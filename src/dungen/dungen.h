#ifndef MINI_DUNGEN_H
#define MINI_DUNGEN_H

#define MIN_ROOM_COUNT 4
#define MAX_ROOM_COUNT 8

#define DUN_SIZE 32
#define DUN_ROOM_SIZE 8
#define DUN_TOTAL_CELLS ((DUN_SIZE / DUN_ROOM_SIZE) * (DUN_SIZE / DUN_ROOM_SIZE))

typedef enum {
    FLOOR = 0,
    WALL,
    DOOR
} tile_type;

typedef enum {
    ROOM_START = 0,
    ROOM_CAVE,
    ROOM_CROSS,
    ROOM_RECT,
    ROOM_SQUARE,
    ROOM_TOTAL
} room_type;

typedef struct {
    tile_type layout[DUN_ROOM_SIZE][DUN_ROOM_SIZE];
} Room;

extern tile_type dungeon[DUN_SIZE][DUN_SIZE];

int dungeon_generate(void);
int get_terrain_char(tile_type tile);

#endif