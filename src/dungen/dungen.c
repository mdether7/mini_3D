#include "dungen.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>   
#include <limits.h>

#include "dice.h"

//////////////////
// Final dungeon
tile_type dungeon[DUN_SIZE][DUN_SIZE];

typedef struct {
    int x, y;
    int cx, cy;
    int order;
    room_type type;
    bool placed_doors[4];
    bool used;
} RoomCell;

typedef enum {
  DUN_NORTH = 0,
  DUN_EAST,
  DUN_SOUTH,
  DUN_WEST
} dun_direction;

static RoomCell room_cells[DUN_TOTAL_CELLS];
extern Room     room_templates[ROOM_TOTAL];

static void fill_map_tiles(tile_type type);
static void reset_room_cells(void);
static void connect_rooms(const RoomCell* room_first, const RoomCell* room_second);
static inline void draw_line(int start, int end, int fixed, bool axis_x);
static inline RoomCell* get_cell_at(int x_pos, int y_pos);
static RoomCell* get_neighbouring_room_cell(const RoomCell* room, dun_direction dir);
static void place_doors(RoomCell* room);
static void get_random_room_center(int room_x, int room_y, int* target_x, int* target_y);

static int comp_qsort(const void* x, const void* y);

int dungeon_generate(void)
{
    fill_map_tiles(WALL);
    reset_room_cells();

    /* randomize the number of rooms that will be created */
    int total_rooms = rand_range(MIN_ROOM_COUNT, MAX_ROOM_COUNT);
    int rooms_to_create = total_rooms; /* i need to cache the original count */

    /* Choose random room cell for starting room  */
    int random_cell = dice_zero(DUN_TOTAL_CELLS);

    room_cells[random_cell].type = ROOM_START;
    room_cells[random_cell].used = true; /* first room will always "fit" */
    room_cells[random_cell].order = 1; /* track order of creation for later iteration  */

    for (int room_x = 0 ;room_x < DUN_ROOM_SIZE; room_x++) {
        for (int room_y = 0 ;room_y < DUN_ROOM_SIZE; room_y++) {
            dungeon[room_x + room_cells[random_cell].x]
                   [room_y + room_cells[random_cell].y] =
            room_templates[ROOM_START].layout[room_x][room_y];
        }
    }

    rooms_to_create--; /* after we create starting room */
    int room_index = 2;

    while (rooms_to_create) {

        random_cell = dice_zero(DUN_TOTAL_CELLS);

        if (room_cells[random_cell].used)
            continue;

        int random_room = dice(ROOM_TOTAL - 1); 
        room_cells[random_cell].type = (room_type)random_room;
        room_cells[random_cell].used = true;

        for (int room_x = 0; room_x < DUN_ROOM_SIZE; room_x++) {
            for (int room_y = 0; room_y < DUN_ROOM_SIZE; room_y++) {
            dungeon[room_x + room_cells[random_cell].x]
                   [room_y + room_cells[random_cell].y] = 
            room_templates[random_room].layout[room_x][room_y];
            }
        }

        rooms_to_create--;
        room_cells[random_cell].order = room_index++; /* keep indexing */

    }

    /**
    * at the end sort room_cells in order of creation
    * for later iteration (rhymes:D) 
    */
    qsort(room_cells, DUN_TOTAL_CELLS, sizeof(RoomCell), comp_qsort);

    /**
    * Now generate random center for each room from where
    * the corridors will start to be layed down
    */
    for (int room = 0; room < total_rooms; room++) {
        get_random_room_center( room_cells[room].x,
                                room_cells[room].y,
                                &room_cells[room].cx,
                                &room_cells[room].cy );
    }

   /**
   * now connect all the rooms with each other 
   * in order of creation so that all rooms get connected. 
   * NOTE(mdether7): total_rooms - 1 needed, else out of bounds
   */
    for (int room = 0; room < total_rooms - 1; room++)                    
        connect_rooms(&room_cells[room], &room_cells[room+1]);

    for (int room = 0; room < total_rooms; room++)
        place_doors(&room_cells[room]);

    return 0;
}

int get_terrain_char(tile_type tile)
{
    switch (tile)
    {
        case FLOOR: return '.';
        case WALL: return '#';
        case DOOR: return '+';
        default: return '?';
    }
}

static void fill_map_tiles(tile_type type)
{
    for (int x = 0; x < DUN_SIZE; x++) {
        for (int y = 0; y < DUN_SIZE; y++) {
            dungeon[x][y] = type;
        } 
    }
}

static void get_random_room_center(int room_x, int room_y,
                                   int* target_x, int* target_y)
{
    int r = dice_zero(4);
    int count = 0;
    for (int i = 3; i < 5; i++) {
        for (int j = 3; j < 5; j++) {
            if ( r == count ) {
                *target_x = room_x + i;
                *target_y = room_y + j;
                return;
            }
            count++;
        }
    }
}


static void reset_room_cells(void)
{
  int count = 0;
  for (int i = 0; i < DUN_SIZE; )
  {
    for (int j = 0; j < DUN_SIZE; )
    {
      room_cells[count++] = (RoomCell){i, j, 0, 0, INT_MAX, 0, {false, false, false, false}, false};
      j+= DUN_ROOM_SIZE;
    }
    i+=DUN_ROOM_SIZE;
  }
}

static inline void draw_line(int start, int end, int fixed, bool axis_x)
{ 
    for (int i = 0; i <= abs(end - start); i++) {
        int pos = start < end ? start + i : start - i;
        if (axis_x)
            dungeon[pos][fixed] = FLOOR;
        else
            dungeon[fixed][pos] = FLOOR;
    }
}

static void connect_rooms(const RoomCell* room_first, const RoomCell* room_second)
{
    int horizontal_first = dice(1);

    if (horizontal_first) {
        draw_line(room_first->cx, room_second->cx, room_first->cy, 1); // horizontal first
        draw_line(room_first->cy, room_second->cy, room_second->cx, 0); // then vertical
    } else {
        draw_line(room_first->cy, room_second->cy, room_first->cx, 0); // vertical first
        draw_line(room_first->cx, room_second->cx, room_second->cy, 1); // then horizontal
    }
}

static RoomCell* get_neighbouring_room_cell(const RoomCell* room, dun_direction dir)
{
    assert(room && "Room is null!");

    int neighbour_x;
    int neighbour_y;

    switch (dir)
    {
    case DUN_NORTH: /* /\ */
        if ((room->x - DUN_ROOM_SIZE) < 0)
            return NULL;

        neighbour_x = room->x - DUN_ROOM_SIZE;
        neighbour_y = room->y;
    break;

    case DUN_EAST: /* -> */
        if ((room->y + DUN_ROOM_SIZE) > DUN_SIZE)
            return NULL;

        neighbour_x = room->x;
        neighbour_y = room->y + DUN_ROOM_SIZE;
    break;

    case DUN_SOUTH: /* \/ */
        if ((room->x + DUN_ROOM_SIZE) > DUN_SIZE)
            return NULL;

        neighbour_x = room->x + DUN_ROOM_SIZE;
        neighbour_y = room->y;
    break;

    case DUN_WEST: /* <- */
        if ((room->y - DUN_ROOM_SIZE) < 0)
            return NULL;

        neighbour_x = room->x;
        neighbour_y = room->y - DUN_ROOM_SIZE;
    break;
    default:
      abort();  
    }

  return get_cell_at(neighbour_x, neighbour_y);
}

static void place_doors(RoomCell* room)
{
  /**
   * Iterate over each room side [N,W,S,E] and place 
   * doors based on certain conditions
   * Also it avoid duplicate doors.
   */
  for (int nwse = 0; nwse < DUN_ROOM_SIZE; nwse++) {    

    if (!dungeon[room->x][room->y+nwse]) { /* north wall */
      RoomCell* north_room = get_neighbouring_room_cell(room, DUN_NORTH);
      if (north_room && !north_room->placed_doors[DUN_SOUTH])
      { /* if north room generated and south wall doesnt yet have walls */
        dungeon[room->x][room->y+nwse] = DOOR;
        room->placed_doors[DUN_NORTH] = true;
      }
    } 

    if (!dungeon[room->x+DUN_ROOM_SIZE - 1][room->y+nwse]) { /* south wall */
      RoomCell* south_room = get_neighbouring_room_cell(room, DUN_SOUTH);
      if (south_room && !south_room->placed_doors[DUN_NORTH]) 
      {
        dungeon[room->x+DUN_ROOM_SIZE - 1][room->y+nwse] = DOOR;
        room->placed_doors[DUN_SOUTH] = true;
      }
    }

    if (!dungeon[room->x+nwse][room->y]) { /* west wall */
      RoomCell* west_room = get_neighbouring_room_cell(room, DUN_WEST);
      if (west_room && !west_room->placed_doors[DUN_EAST])
      {
        dungeon[room->x+nwse][room->y] = DOOR;
        room->placed_doors[DUN_WEST] = true;
      }
    }

    if (!dungeon[room->x+nwse][room->y+DUN_ROOM_SIZE - 1]) { /* east wall */
      RoomCell* east_room = get_neighbouring_room_cell(room, DUN_EAST);
      if (east_room && !east_room->placed_doors[DUN_WEST])
      {
        dungeon[room->x+nwse][room->y+DUN_ROOM_SIZE - 1] = DOOR;
        room->placed_doors[DUN_EAST] = true;
      } 
    }

  }

}

static inline RoomCell* get_cell_at(int x_pos, int y_pos)
{
    assert((x_pos >= 0 && x_pos <= DUN_SIZE - DUN_ROOM_SIZE));
    assert((y_pos >= 0 && y_pos <= DUN_SIZE - DUN_ROOM_SIZE));
    assert((x_pos % DUN_ROOM_SIZE == 0));
    assert((y_pos % DUN_ROOM_SIZE == 0));

    for (int cell = 0; cell < DUN_TOTAL_CELLS; cell++) {
        if (room_cells[cell].x == x_pos && room_cells[cell].y == y_pos)
            return &room_cells[cell];
    }

    assert(0 && "Room at valid grid position should always be found in room_cells array!");
    return NULL;
}

static int comp_qsort(const void* x, const void* y)
{
    RoomCell* cellx = (RoomCell*)x;
    RoomCell* celly = (RoomCell*)y;
    
    return cellx->order - celly->order;
}
