#include "platform_input.h"
#include <GLFW/glfw3.h>
#include <assert.h>

Key keys[KEY_COUNT];

int platform_is_key_pressed(key_type key)
{
    return keys[key].pressed;
}

int platform_is_key_released(key_type key)
{
    return keys[key].released;
}

int platform_is_key_held(key_type key)
{
    return keys[key].held;
}

int platform_is_key_down(key_type key)
{
    return keys[key].down;
}


void platform_input_set_key(key_type type, int action)
{
    assert((type >= KEY_A && type < KEY_COUNT));
    switch(action)
    {
        case GLFW_PRESS:
        {
            keys[type].pressed = 1;
            keys[type].down = 1;
        } break;
        case GLFW_REPEAT:
        {
            keys[type].held = 1;
        } break;
        case GLFW_RELEASE:
        {
            keys[type].released = 1;
        } break;
        default: {
            assert(0 && "Unknown button action");
        } break;
    }
}

void platform_input_reset_keys_state(void)
{
    for (int i = 0; i < KEY_COUNT; i++) {

        if (keys[i].released) {
            keys[i].held = 0;
            keys[i].down = 0;
        }

        keys[i].pressed  = 0;
        keys[i].released = 0;
    }
}