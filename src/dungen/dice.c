#include "dice.h"

#include <stdlib.h>
#include <time.h>
#include <assert.h>

void dice_rnum_init(char* running_state)
{
	time_t t = time(NULL);
	t ^= (getuid() << 16) | (getpid() & 0xFFFF);
#ifdef DEBUG
	t = 1234;
#endif
	initstate(t, running_state, 256);
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
  return ((double)rand() / (RAND_MAX + 1.0)) < p;
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
  while ( (rval = rand()) >= limit );

  return rval % sides + 1;
}

int dice_zero(int sides)
{
  int rval;
  int limit;
  
  limit = RAND_MAX - (RAND_MAX % sides);
  while ( (rval = rand()) >= limit );

  return rval % sides;
}

int rand_range(int min, int max)
{
  return min + dice_zero(max - min + 1);
}