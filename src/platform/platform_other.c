#include "platform_other.h"

#include <GLFW/glfw3.h>
#include <assert.h>

extern GLFWwindow* window;

void platform_get_window_size(int result[2])
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    result[0] = w;
    result[1] = h;
}

void platform_get_framebuffer_size(int result[2])
{
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    result[0] = w;
    result[1] = h;
}