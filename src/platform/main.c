#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "GLFW/glfw3.h"

// Nuklear 
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "Nuklear/nuklear.h"

#define NK_GLFW_GL3_IMPLEMENTATION
#include "Nuklear/nuklear_glfw_gl3.h"

// Standard Library
#include <stdlib.h>
#include <assert.h> // for test, maybe remove?

// Platform
#include "platform_log.h"
#include "platform_input.h"
#include "platform_debug.h"
#include "platform_tools.h"

#ifdef DEBUG
#warning "Debug build enabled!"
#endif

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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    for (int button = GLFW_KEY_A; button <= GLFW_KEY_Z; button++) {
        if (key == button) {
            platform_input_set_key(button - GLFW_KEY_A, action);
        }
    }

#if 0
    platform_log_info("KEY: %d (scancode %d), action %d, mods %d",
            key, scancode, action, mods);
#endif

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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); 
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
#ifdef ENABLE_GL_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
    
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

#ifdef ENABLE_GL_DEBUG
    debug_gl_enable();
    debug_display_information();
#endif

    // initialize nuklear.
    // initialize game.

    // fps counter / delta time.
    while (!glfwWindowShouldClose(window))
    {
        // update.
        if (platform_is_key_down(KEY_W)) {
            platform_log_info("UP");
        }
        if (platform_is_key_down(KEY_S)) {
            platform_log_info("DOWN");
        } 
        if (platform_is_key_down(KEY_A)) {
            platform_log_info("LEFT");
        }
        if (platform_is_key_down(KEY_D)) {
            platform_log_info("RIGHT");
        }

        // render.

        glfwSwapBuffers(window);
        platform_input_reset_keys_state();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}







