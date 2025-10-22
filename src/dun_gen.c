#include "dun_gen.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#include "dun_rooms.h"
#include "dungen/dice.h"

enum tile_type maptiles[DUN_SIZE][DUN_SIZE];

struct placed_doors {
  bool door_n, door_e, door_s, door_w;
};

typedef struct { /* should be internal to dungen.c */
  int x, y;
  int cx, cy;
  int order;
  enum room_type type;
  struct placed_doors doors;
  bool used;
} room_cell;

enum direction {
  NORTH = 0,
  EAST,
  SOUTH,
  WEST
};

static room_cell room_cells[DUN_TOTAL_CELLS]; /* array internal to dungen */

static void              reset_room_cells(void);
static void              fill_map_tiles(enum tile_type type);
static inline void       draw_line(int start, int end, int fixed, bool axis_x);
static void              connect_rooms(const room_cell* room_first, const room_cell* room_second);
static void              get_random_room_center(int room_x, int room_y, int* target_x, int* target_y);
static int               comp_qsort(const void* x, const void* y);
static inline            room_cell* get_cell_at(int x_pos, int y_pos);
#ifdef PLACE_DOORS
static void              place_doors(room_cell* room); /* modifies placed_doors member */
static                   room_cell* get_neighbouring_room_cell(const room_cell* room, enum direction dir);
#endif


static inline room_cell* get_cell_at(int x_pos, int y_pos)
{
  assert((x_pos >= 0 && x_pos <= DUN_SIZE - DUN_ROOM_SIZE));
  assert((y_pos >= 0 && y_pos <= DUN_SIZE - DUN_ROOM_SIZE));
  assert((x_pos % DUN_ROOM_SIZE == 0));
  assert((y_pos % DUN_ROOM_SIZE == 0));
  for (int cell = 0; cell < DUN_TOTAL_CELLS; cell++)
  {
    if (room_cells[cell].x == x_pos && room_cells[cell].y == y_pos) {
      return &room_cells[cell];
    }
  }
  assert(0 && "Room at valid grid position should always be found in room_cells array!");
  return NULL;
}

#ifdef PLACE_DOORS
static room_cell* get_neighbouring_room_cell(const room_cell* room, enum direction dir)
{
  assert(room && "Room is null!");

  int neighbour_x;
  int neighbour_y;

  switch (dir)
  {
    case NORTH: /* /\ */
      if ((room->x - DUN_ROOM_SIZE) < 0) {
        return NULL;
      }

      neighbour_x = room->x - DUN_ROOM_SIZE;
      neighbour_y = room->y;
      break;

    case EAST: /* -> */
      if ((room->y + DUN_ROOM_SIZE) > DUN_SIZE) {
        return NULL;
      }

      neighbour_x = room->x;
      neighbour_y = room->y + DUN_ROOM_SIZE;
      break;

    case SOUTH: /* \/ */
      if ((room->x + DUN_ROOM_SIZE) > DUN_SIZE) {
        return NULL;
      }

      neighbour_x = room->x + DUN_ROOM_SIZE;
      neighbour_y = room->y;
      break;

    case WEST: /* <- */
      if ((room->y - DUN_ROOM_SIZE) < 0) {
        return NULL;
      }

      neighbour_x = room->x;
      neighbour_y = room->y - DUN_ROOM_SIZE;
      break;

    default:
      abort();  
  }
  return get_cell_at(neighbour_x, neighbour_y);
}
#endif

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

  for (int room_x = 0 ;room_x < DUN_ROOM_SIZE; room_x++)
  {
    for (int room_y = 0 ;room_y < DUN_ROOM_SIZE; room_y++)
    {
      maptiles[room_x + room_cells[random_cell].x]
              [room_y + room_cells[random_cell].y] =
      room_templates[ROOM_START].layout[room_x][room_y];
    }
  }

  rooms_to_create--; /* after we create starting room */
  int room_index = 2;

  while (rooms_to_create) {

    random_cell = dice_zero(DUN_TOTAL_CELLS);

    if (room_cells[random_cell].used) {
      continue;
    }

    int random_room = dice(ROOM_TOTAL - 1); 
    room_cells[random_cell].type = (enum room_type)random_room;
    room_cells[random_cell].used = true;

    for (int room_x = 0; room_x < DUN_ROOM_SIZE; room_x++)
    {
      for (int room_y = 0; room_y < DUN_ROOM_SIZE; room_y++)
      {
        maptiles[room_x + room_cells[random_cell].x]
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
  qsort(room_cells, DUN_TOTAL_CELLS, sizeof(room_cell), comp_qsort);

  /**
   * Now generate random center for each room from where
   * the corridors will start to be layed down
   */
  for (int room = 0; room < total_rooms; room++)
  {
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
  {                    
    connect_rooms(&room_cells[room], &room_cells[room+1]);
  }
#ifdef PLACE_DOORS
  for (int room = 0; room < total_rooms; room++)
  {
    place_doors(&room_cells[room]);
  }
#endif
  return 0;
}

/* IFNDEF STUDY */
/* DEF STUDY */

static inline void draw_line(int start, int end, int fixed, bool axis_x)
{ 
  for (int i = 0; i <= abs(end - start); i++) {
      int pos = start < end ? start + i : start - i;
      if (axis_x)
          maptiles[pos][fixed] = FLOOR;
      else
          maptiles[fixed][pos] = FLOOR;
  }
}

static void connect_rooms(const room_cell* room_first, const room_cell* room_second)
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

/* ENDIF STUDY */

#ifdef PLACE_DOORS
static void place_doors(room_cell* room)
{
  /**
   * Iterate over each room side [N,W,S,E] and place 
   * doors based on certain conditions
   * Also it avoid duplicate doors.
   */

  for (int nwse = 0; nwse < DUN_ROOM_SIZE; nwse++)
  {    

    if (!maptiles[room->x][room->y+nwse]) { /* north wall */
      room_cell* north_room = get_neighbouring_room_cell(room, NORTH);
      if (north_room && !north_room->doors.door_s)
      { /* if north room generated and south wall doesnt yet have walls */
        maptiles[room->x][room->y+nwse] = DOOR;
        room->doors.door_n = true;
      }
    } 

    if (!maptiles[room->x+DUN_ROOM_SIZE - 1][room->y+nwse]) { /* south wall */
      room_cell* south_room = get_neighbouring_room_cell(room, SOUTH);
      if (south_room && !south_room->doors.door_n) 
      {
        maptiles[room->x+DUN_ROOM_SIZE - 1][room->y+nwse] = DOOR;
        room->doors.door_s = true;
      }
    }

    if (!maptiles[room->x+nwse][room->y]) { /* west wall */
      room_cell* west_room = get_neighbouring_room_cell(room, WEST);
      if (west_room && !west_room->doors.door_e)
      {
        maptiles[room->x+nwse][room->y] = DOOR;
        room->doors.door_w = true;
      }
    }

    if (!maptiles[room->x+nwse][room->y+DUN_ROOM_SIZE - 1]) { /* east wall */
      room_cell* east_room = get_neighbouring_room_cell(room, EAST);
      if (east_room && !east_room->doors.door_w)
      {
        maptiles[room->x+nwse][room->y+DUN_ROOM_SIZE - 1] = DOOR;
        room->doors.door_e = true;
      } 
    }

  }

}
#endif

static void fill_map_tiles(enum tile_type type)
{
  for (int x = 0; x < DUN_SIZE; x++)
  {
    for (int y = 0; y < DUN_SIZE; y++)
    {
      maptiles[x][y] = type;
    }
  }
}

static void get_random_room_center(int room_x, int room_y,
                                   int* target_x, int* target_y)
{
  int r = dice_zero(4);
  int count = 0;
  for (int i = 3; i < 5; i++)
  {
    for (int j = 3; j < 5; j++)
    {
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
      room_cells[count++] = (room_cell){i, j, 0, 0, INT_MAX, 0, {false, false, false, false}, false};
      j+= DUN_ROOM_SIZE;
    }
    i+=DUN_ROOM_SIZE;
  }
}

static int comp_qsort(const void* x, const void* y)
{
  room_cell* cellx = (room_cell*)x;
  room_cell* celly = (room_cell*)y;
  
  return cellx->order - celly->order;
}

