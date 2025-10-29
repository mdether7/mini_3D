#include "platform_glfw.h"

#define GLFW_INCLUDE_NONE
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "platform_log.h"
#include "platform_debug.h"

GLFWwindow* window = NULL;
int fullscreen     = 0;

static void error_callback(int error, const char* description)
{
    platform_log_error("[GLFW] (%d): %s", error, description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

int platform_initialize(const char* window_name, int w, int h)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        return 1;
    }
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); 
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    window = glfwCreateWindow(w, h, window_name, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // enable vsync

    glfwSetKeyCallback(window, key_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        glfwTerminate();
        platform_log_error("[GLAD] Failed to initialize!");
        return 1;
    }
    
    debug_gl_enable();    
    debug_display_information();

    return 0;
}

void platform_present_frame(void)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int platform_window_shoudl_close(void)
{
    return glfwWindowShouldClose(window);
}







