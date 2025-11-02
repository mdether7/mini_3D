#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Nuklear 
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include <Nuklear/nuklear.h>

#define NK_GLFW_GL3_IMPLEMENTATION
#include <Nuklear/nuklear_glfw_gl3.h>

// Standard Library
#include <stdlib.h>

// Platform
#include "platform_log.h"
#include "platform_input.h"
#include "platform_debug.h"
#include "platform_tools.h"

// Game
#include "game/game_main.h"

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

    // for (int button = GLFW_KEY_A; button <= GLFW_KEY_Z; button++) {
    //     if (key == button) {
    //         platform_input_set_key(button - GLFW_KEY_A, action);
    //     }
    // }    [DRILL THIS INTO YOUR EYES!]

    if (key >= GLFW_KEY_A && key <= GLFW_KEY_Z) {
        platform_input_set_key(key - GLFW_KEY_A, action);
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
    
    // Scissor playground //
    // glViewport(0, 0, window_width / 2, window_height / 2);
    // glScissor(0,0,window_width / 2,window_height/ 2);
    // glEnable(GL_SCISSOR_TEST);
    // glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    if (dg_init()) { // TODO: Here think about proper game cleanup.
        glfwDestroyWindow(window);
        glfwTerminate();
        platform_log_error("[DUNGEN] Failed to initialize!");
        return EXIT_FAILURE;
    }

    // glfwGetFramebufferSize(window, );
    // glfwGetWindowSize(window,)
    
    // fps counter / delta time.
    while (!glfwWindowShouldClose(window))
    {
        float current_time = glfwGetTime();

        dg_loop(current_time); // current time for now.

        platform_input_reset_keys_state();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    dg_close();

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}







