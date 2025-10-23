#ifndef MINI_DICE_H
#define MINI_DICE_H

/**
 * Mini dice random library (POSIX only)
 * -------------------------------------
 * Uses random(), initstate(), and setstate()
 *
 * Functions:
 *  - dice_state_init: initialize RNG state array (must be called before use)
 *  - dice_set_state: switch active RNG state
 *  - dice: roll dice in range [1, sides]
 *  - dice_zero: roll dice in range [0, sides-1]
 *  - dice_chance: probabilistic event (returns 1 with probability p)
 *  - dice_multiple: roll N dice and sum the results
 *  - dice_zero_multiple: same but 0-based
 *  - rand_range: general [min, max] random integer
 */

char* dice_init_state(char* state); /* This must be called before use, otherwise state[128], seed=1 */
char* dice_set_state(char* state);

int  dice(int sides); /* dice range 1 to SIDES */
int  dice_chance(double p); /* Returns 1 if random event occurs with probability p (0.0 to 1.0) */
int  dice_zero(int sides); /* dice range 0 to SIDES-1 */
int  dice_multiple(int count, int sides);
int  dice_zero_multiple(int count, int sides); 
int  rand_range(int min, int max);

#endif
