#include "platform_input.h"
#include <GLFW/glfw3.h>
#include <assert.h>

Key keys[KEY_COUNT];
int mouse_moved = 1;
double mouse_dx;
double mouse_dy;

extern GLFWwindow* window;

void platform_input_get_cursor_pos(double* xpos, double* ypos)
{
   glfwGetCursorPos(window, xpos, ypos);
}

void platform_input_get_mouse_delta(double* dx, double* dy)
{
    *dx = mouse_dx;
    *dy = mouse_dy;
}

void platform_input_show_cursor(void)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void platform_input_disable_cursor(void)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

int platform_input_mouse_moved(void)
{
    return mouse_moved;
}

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

    mouse_dx = 0.0;
    mouse_dy = 0.0;
    mouse_moved = 0;
}