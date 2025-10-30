#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <stdlib.h>

#include "platform_log.h"
#include "platform_debug.h"

GLFWwindow* window = NULL;
int fullscreen     = 0;
int window_width   = 1280;
int window_height  = 720;

static void error_callback(int error, const char* description)
{
    platform_log_error("[GLFW] (%d): %s", error, description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

int main(void)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        platform_log_error("[GLFW] Failed to initialize!");
        return EXIT_FAILURE;
    }


    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); 
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    
    window = glfwCreateWindow(window_width, window_height, "DunGen", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // enable vsync


    glfwSetKeyCallback(window, key_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        platform_log_error("[GLAD] Failed to initialize!");
        return EXIT_FAILURE;
    }

    // initialize nuklear.
    // initialize game.

    while (!glfwWindowShouldClose(window))
    {
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}







