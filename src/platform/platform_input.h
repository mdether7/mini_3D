#ifndef PLATFORM_INPUT_H
#define PLATFORM_INPUT_H

typedef enum {
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
    KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y ,KEY_Z, KEY_COUNT,   
} key_type;

typedef struct {
    unsigned char pressed;
    unsigned char held;
    unsigned char released;
    unsigned char down;
} Key;
 
extern Key keys[KEY_COUNT];

void platform_input_get_cursor_pos(double* xpos, double* ypos);
void platform_input_show_cursor(void);
void platform_input_disable_cursor(void);

int platform_is_key_pressed(key_type key);
int platform_is_key_held(key_type key);
int platform_is_key_released(key_type key);
int platform_is_key_down(key_type key);

void platform_input_reset_keys_state(void);
void platform_input_set_key(key_type type, int action);


#endif