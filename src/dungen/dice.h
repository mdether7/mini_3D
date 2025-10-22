#ifndef MINI_DICE_H
#define MINI_DICE_H

void rnum_init(void);
int dice(int sides); /* dice range 1 to SIDES */
int dice_chance(double p); /* Returns 1 if random event occurs with probability p (0.0 to 1.0) */
int dice_zero(int sides); /* dice range 0 to SIDES-1 */
int dice_multiple(int count, int sides);
int dice_zero_multiple(int count, int sides); /* [[MAYBE_UNUSED]] */ 
int rand_range(int min, int max);

#endif