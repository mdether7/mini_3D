#define _DEFAULT_SOURCE
#include "dice.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>

char* dice_init_state(char* state)
{
    time_t t = time(NULL);
    t ^= (getuid() << 16) | (getpid() & 0xFFFF);
    return initstate(t, state, 256);
}

char* dice_set_state(char* state)
{
    return setstate(state);
}

int dice_multiple(int count, int sides)
{
    int total = 0;
    for ( ; count > 0; count--) {
        total += dice(sides);
    }
    return total;
}

int dice_chance(double p) 
{
    assert(p >= 0.0 && p <= 1.0);
    return ((double)random() / (RAND_MAX + 1.0)) < p;
}

int dice_zero_multiple(int count, int sides)
{
    int total = 0;
    for ( ; count > 0; count--) {
        total += dice_zero(sides);
    }
    return total;
}

int dice(int sides)
{
    int rval;
    int limit;
    
    limit = RAND_MAX - (RAND_MAX % sides);
    while ( (rval = random()) >= limit );

    return rval % sides + 1;
}

int dice_zero(int sides)
{
    int rval;
    int limit;
    
    limit = RAND_MAX - (RAND_MAX % sides);
    while ( (rval = random()) >= limit );

    return rval % sides;
}

int rand_range(int min, int max)
{
    return min + dice_zero(max - min + 1);
}
