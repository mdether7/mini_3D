#include "platform_input.h"
#include "GLFW/glfw3.h"

#include <assert.h>

Button buttons[KEY_COUNT];

void platform_internal_input_set_button(const button_type type, int action)
{
    switch(action)
    {
        case GLFW_RELEASE:
        {
            buttons[type].released = 1;
        } break;
        case GLFW_PRESS:
        {
            buttons[type].pressed = 1;
        } break;
        case GLFW_REPEAT:
        {
            buttons[type].held = 1;
        } break;
        default: {
            assert(0 && "Unknown button action");
        } break;
    }
}