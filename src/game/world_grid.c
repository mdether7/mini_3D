#include "world_gird.h"

#include <linmath/linmath.h>
#include "misc/math_misc.h"

#include <stdint.h>

// My imaginary units? MAYBE AS FUNTION PARAMETER / CASEY?

typedef enum {
    QUADRANT_I,
    QUADRANT_II,
    QUADRANT_III,
    QUADRANT_IV
} game_world_quadrant;

// grid_3d_position_to_quadrant:
//         movss   xmm2, DWORD PTR [rdi]
//         pxor    xmm0, xmm0
//         movss   xmm1, DWORD PTR [rdi+8]
//         comiss  xmm2, xmm0
//         jb      .L12
//         comiss  xmm1, xmm0
//         sbb     eax, eax
//         and     eax, 3
//         add     eax, 1
//         ret
// .L12:
//         xor     eax, eax
//         ucomiss xmm1, xmm0
//         setb    al
//         add     eax, 2
//         ret

game_world_quadrant grid_3d_position_to_quadrant(vec3 pos)
{
    float x_axis = pos[0];
    float z_axis = pos[1];
    game_world_quadrant result = QUADRANT_I;
    if (x_axis >= 0.0f) 
    {
        if (z_axis >= 0.0f) result = QUADRANT_I;
        else                result = QUADRANT_IV;
    }
    else 
    {
        if (z_axis >= 0.0f) result = QUADRANT_II;
        else                result = QUADRANT_III;
    }
    return result;
}

game_world_quadrant grid_3d_position_to_quadrant(vec3 pos) {
    // fastest possible assembly?
    int xi = pos[0] < 0;
    int zi = pos[2] < 0;
    int idx = (xi << 1) | zi; // TODO STUDY, DONT STEAL FROM CLAUDE WITHOUT UNDERSTANDING
    
    static const game_world_quadrant table[4] = {
        QUADRANT_I,   // x >= 0, z >= 0
        QUADRANT_IV,  // x >= 0, z < 0
        QUADRANT_II,  // x < 0,  z >= 0
        QUADRANT_III  // x < 0,  z < 0
    };
    
    return table[idx];
}

int grid_3d_position_to_quadrant(float pos[3])
{
    float x_axis = pos[0];
    float z_axis = pos[1];

    if (x_axis >= 0.0f) 
    {
        if (z_axis >= 0.0f) return 1;
        else                return 4;
    }
    else 
    {
        if (z_axis >= 0.0f) return 2;
        else                return 3;
    }
}

int grid_3d_position_to_quadrant(float pos[3])
{
    int result;
    
    if (pos[0] >= 0.0f) {
        result = (pos[2] >= 0.0f) ? 1 : 4;
    } else {
        result = (pos[2] >= 0.0f) ? 2 : 3;
    }
    
    return result;
}

void grid_3d_position_to_sector(const vec3 pos, unsigned int sector_size, vec2 out_sector)
{
    vec3 wordl_origin;
    mathm_vec3_zero(wordl_origin);

}