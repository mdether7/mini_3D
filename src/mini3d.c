///////////////////////
// Standard C headers
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>

#define GLFW_INCLUDE_NONE // <- glad/GLFW can be included in any order
#define NK_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#define MINI_MATH_TYPE_LIN

//////////////////
// External libs
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifdef MINI_MATH_TYPE_LIN
#include <linmath/linmath.h>
#else
#include <cglm/call.h>
#endif

#include "Nuklear/nuklear.h"
#include "stb/stb_image.h"

///////////////
// My headers
#ifdef DEBUG
#warning "Debug build enabled!"
#endif

// Debug math tools (Linmath only)
#if defined(DEBUG) && defined(MINI_MATH_TYPE_LIN)
#warning "Leaving this for the memorization :D"
#endif

#include "gl_helpers.h"
#include "math_helpers.h"

///////////////////////////////////////////
//
//  My macros
//
///////////////////////////////////////////

#define UNUSED(var) (void)var

#define WINDOW_MAX_NAME_LEN 64
#define WINDOW_DEFAULT_WIDTH 1280
#define WINDOW_DEFAULT_HEIGHT 800

///////////////////////////////////////////
//
//  Structs
//
///////////////////////////////////////////

typedef struct s_window_settings {
    char  name[WINDOW_MAX_NAME_LEN];
    int   width;
    int   height;
} WindowSettings;

typedef struct s_frame_counter {
    double ms_per_frame;
    double avg_fps;
} FrameCounter;

typedef struct s_user_config {
    bool   should_start_maximized;
    bool   should_start_vsync;
} UserConfig;

///////////////////////////////////////////
//
//  Globals
//
///////////////////////////////////////////

static WindowSettings g_window_setting = {
    .width  = WINDOW_DEFAULT_WIDTH,
    .height = WINDOW_DEFAULT_HEIGHT,
    .name   = "mini 3D",
};

static UserConfig g_user_config = {
    .should_start_maximized     = false,
    .should_start_vsync         = true,
};

static FrameCounter g_frame_counter = {
    .ms_per_frame = 0.0,
    .avg_fps      = 0.0,
};

///////////////////////////////////////////
//
//  Functionality hopefully XD
//
///////////////////////////////////////////

static void
mini_die(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}

static void
mini_print_n_flush(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    fputs("\n", stdout);
    fflush(stdout);
}

static void
mini_update_framecounter(FrameCounter* counter, double ms_per_frame)
{   
    counter->ms_per_frame = ms_per_frame;
    counter->avg_fps = 1000.0 / ms_per_frame;
}


///////////////////////////////////////////
//
//  GLFW callbacks
//
///////////////////////////////////////////

static void error_callback(int error, const char* description) { fprintf(stderr, "Error: %s\n", description); }
// Optional: Update your projection matrix here too (aspect ratio)
void framebuffer_size_callback(GLFWwindow* window, int width, int height) { 
    glViewport(0, 0, width, height);
    // those width and height are the framebuffer sizes not window ones
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Thats crazy useful! // 
    // int* my_int = (int*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

///////////////////////////////////////////
//
//  Main finally.
//
///////////////////////////////////////////

int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    GLFWwindow*  window = NULL;

    /* GLFW */
#ifdef __linux__
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);
#endif
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        const char* error;
        glfwGetError(&error);
        mini_die("[GLFW] %s", error);
    }
    
    /* Window creation hints */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  

    if (g_user_config.should_start_maximized) {
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    } 

#ifdef ENABLE_GL_DEBUG_OUTPUT
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); 
#endif

    /* create window */
    window = glfwCreateWindow(g_window_setting.width, g_window_setting.height, g_window_setting.name, NULL, NULL);
    if (window == NULL) { 
        const char* error;
        glfwGetError(&error);
        glfwTerminate();
        mini_die("[GLFW] %s", error);
    }

    /* Make this window the gl context */
    glfwMakeContextCurrent(window);
    g_user_config.should_start_vsync ? glfwSwapInterval(1) : glfwSwapInterval(0);

    /* GLFW callbacks setup */
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* openGL */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwTerminate();
        mini_die("[GLAD] Failed to load openGL functions!");
    }

    {
        /* Set initial viewport */
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
    }

#ifdef ENABLE_GL_DEBUG_OUTPUT
    {
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(gl_debug_output_callback, NULL);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                 GL_DONT_CARE, 0, NULL, GL_TRUE);
            mini_print_n_flush("[GL] Debug output enabled");
        }
    }
#endif
    /* Load OpenGL Stuff */

    GLuint default_program = shader_program_compile("shaders/default.vert",
                                                  "shaders/default.frag"); 
    if (default_program == 0) {
        glfwTerminate();
        mini_die("[GL] Shader compilation failed!");
    }

    const GLfloat triangle[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(triangle), triangle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    /* Playgroud stuff, might be messy! */

    {
        mat4x4 model;
        mat4x4_identity(model);

        mat4x4_scale_aniso(model, model, 0.5f, 0.5f, 0.5f);
        // mat4x4_rotate_Z(model, model, );

        float degree = 1.0f;
        float radian = 1.0f;

        fprintf(stdout, "D TO R: %f\n", mini_degrees_to_radians(degree));
        fprintf(stdout, "R TO D: %f\n", mini_radians_to_degrees(radian));
        fflush(stdout);


        
        mini_math_print_mat4x4(model);
        // mat4x4_translate(MVP, 0.2f, 0.2f, 0.0f);
        // mini_math_print_mat4x4(MVP);

        GLint uniform_loc = glGetUniformLocation(default_program, "model");
        glUseProgram(default_program);
        glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, &model[0][0]);
    }



    /* OpenGL initial options setup */
    glClearColor(0.5f, 0.5f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    double last_time = glfwGetTime();
    int frames = 0;
    while (!glfwWindowShouldClose(window))
    {
        double current_time = glfwGetTime();
        frames++;
        if ( current_time - last_time >= 1.0) {
            mini_update_framecounter(&g_frame_counter, 1000.0/(double)frames);
            mini_print_n_flush("%.2f ms/frame | %.2f FPS", 
                g_frame_counter.ms_per_frame, g_frame_counter.avg_fps);
            frames = 0;
            last_time += 1.0;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glUseProgram(default_program);
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 3);
       
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* OpenGL objects cleanup */
    glDeleteProgram(default_program);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // no need to destroy technically.
    glfwDestroyWindow(window);

    glfwTerminate();
    return EXIT_SUCCESS;
}