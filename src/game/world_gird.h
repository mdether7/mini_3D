#ifndef DG_WORLD_GRID_H
#define DG_WORLD_GRID_H

#include <linmath/linmath.h>

typedef enum {
    I,
    II,
    III,
    IV
} game_world_quadrant;

void grid_3d_position_to_sector(const vec3 pos, unsigned int sector_size, vec2 out_sector);
game_world_quadrant grid_3d_position_to_quadrant(vec3 pos);

#endif