///////////////////////
// Standard C headers
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <float.h>

#define GLFW_INCLUDE_NONE // <- glad/GLFW can be included in any order
#define NK_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#define MINI_MATH_TYPE_LIN

//////////////////
// External libs
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#ifdef MINI_MATH_TYPE_LIN
#include "linmath/linmath.h"
#else
#include "cglm/call.h"
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

#include "mini_utils.h"
#include "gl_helpers.h"
#include "math_helpers.h"
#include "mini_data.h"

///////////////////////////////////////////
//
//  My macros
//
///////////////////////////////////////////

#define UNUSED(var) (void)var

#define WINDOW_MAX_NAME_LEN 64
#define WINDOW_DEFAULT_WIDTH 1280
#define WINDOW_DEFAULT_HEIGHT 800

#define MAX_SHADER_PATH 1024

///////////////////////////////////////////
//
//  Enums
//
///////////////////////////////////////////

typedef enum {
    PROGRAM_SLOT_0,
    PROGRAM_SLOT_1,
    PROGRAM_SLOT_2,
    PROGRAM_SLOT_3,
    PROGRAM_SLOT_4,
    PROGRAM_SLOT_5,
    PROGRAM_SLOT_6,
    PROGRAM_SLOT_7,
    PROGRAM_SLOT_8,
    PROGRAM_SLOT_9,
    MAX_SHADER_PROGRAMS
} ProgramType;

typedef enum {
    ACTION_NONE = -1,
    ACTION_MOVE_FORWARD,
    ACTION_MOVE_BACKWARD,
    ACTION_MOVE_LEFT,
    ACTION_MOVE_RIGHT,
    ACTION_COUNT
} GameAction;

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    NWSE_COUNT,
} Direction;

///////////////////////////////////////////
//
//  Structs
//
///////////////////////////////////////////

typedef struct s_window_state {
    char  name[WINDOW_MAX_NAME_LEN];
    int   width;
    int   height;
    bool  resized;
} WindowState;

typedef struct s_shader_program {
    GLuint program_handle;
    char vertex_shader_path[MAX_SHADER_PATH];
    char fragment_shader_path[MAX_SHADER_PATH];
} ShaderProgram;

typedef struct s_frame_counter {
    double ms_per_frame;
    double avg_fps;
} FrameCounter;

typedef struct s_user_config {
    bool   should_start_maximized;
    bool   should_start_vsync;
    bool   should_start_focused;
} UserConfig;

typedef struct s_settings {
    float mouse_sensitivity;
} Settings;

typedef struct s_input_state {
    bool actions[ACTION_COUNT];
} InputState;

typedef struct s_camera {
    mat4x4 view;
    mat4x4 projection;
    vec3  pos;
    vec3  direction;
    vec3  up;
    float yaw;
    float pitch;
    float fov;
    float speed; // should be here??
    float near;  // ?? (i'll leave it for now)
    float far;   // ??
} Camera;

///////////////////////////////////////////
//
//  Globals
//
///////////////////////////////////////////

static WindowState g_window_state = {
    .width   = WINDOW_DEFAULT_WIDTH,
    .height  = WINDOW_DEFAULT_HEIGHT,
    .name    = "mini 3D",
    .resized = false,
};

static UserConfig g_user_config = {
    .should_start_maximized     = false,
    .should_start_vsync         = true,
    .should_start_focused       = true,
};

static FrameCounter g_frame_counter = {
    .ms_per_frame = 0.0,
    .avg_fps      = 0.0,
};

static Settings g_settings = {
    .mouse_sensitivity = 0.05f,
};

static Camera g_camera = {
    .view       = MAT4x4_IDENTITY,
    .projection = MAT4x4_IDENTITY,
    .pos        = (vec3){0.0f, 0.0f, 10.0f},
    .direction  = (vec3){0.0f, 0.0f, -1.0f},
    .up         = (vec3){0.0f, 1.0f, 0.0f},
    .yaw        = -90.0f, // start facing north.
    .pitch      = 0.0f,
    .fov        = 60.0f,
    .speed      = 5.0f,
    .near       = 0.1f, 
    .far        = 100.0f,
};

// For now there's no need for multiple programs, but who knows.
// Also someday replace GLuint with ShaderProgram struct maybeee?
static GLuint g_shader_programs[MAX_SHADER_PROGRAMS];

//////////
// Input
static InputState g_input_state = {
    .actions = {false},
};
                                                // off by one
static GameAction g_input_key_to_action[GLFW_KEY_LAST + 1] = {ACTION_NONE};

////////////////////////
// Cardinal Directions
static const vec3 DIR_FORWARD  = {0.0f, 0.0f, -1.0f}; //north (-z)
static const vec3 DIR_BACKWARD = {0.0f, 0.0f, 1.0f};  //south (+z)
static const vec3 DIR_RIGHT    = {1.0f, 0.0f, 0.0f};  //east (+x)
static const vec3 DIR_LEFT     = {-1.0f, 0.0f, 0.0f}; //west (-x)

///////////////////////////////////////////
//
//  Functionality hopefully XD
//
///////////////////////////////////////////

static void
input_init_keybindings(void)          
{   // Need to do this cus the initialization in global scope is shit.
    for (int i = 0; i <= GLFW_KEY_LAST; i++) // (only first element was
        g_input_key_to_action[i] = ACTION_NONE; // initialized to -1 XD)

    g_input_key_to_action[GLFW_KEY_W] = ACTION_MOVE_FORWARD;
    g_input_key_to_action[GLFW_KEY_S] = ACTION_MOVE_BACKWARD;
    g_input_key_to_action[GLFW_KEY_A] = ACTION_MOVE_LEFT;
    g_input_key_to_action[GLFW_KEY_D] = ACTION_MOVE_RIGHT;

    g_input_key_to_action[GLFW_KEY_UP] = ACTION_MOVE_FORWARD;
    g_input_key_to_action[GLFW_KEY_DOWN] = ACTION_MOVE_BACKWARD;
    g_input_key_to_action[GLFW_KEY_LEFT] = ACTION_MOVE_LEFT;
    g_input_key_to_action[GLFW_KEY_RIGHT] = ACTION_MOVE_RIGHT;
}

static void
mini_print_camera(Camera* camera)
{
    fprintf(stdout, "--- CAMERA ---\n");
    fprintf(stdout, "POS: ");
    mini_math_print_vec3(camera->pos);
    fprintf(stdout, "DIR: ");
    mini_math_print_vec3(camera->direction);
    fprintf(stdout, "UP: ");
    mini_math_print_vec3(camera->up);
    fprintf(stdout, "FOV: %f\n", camera->fov);
    fprintf(stdout, "SPEED: %f\n", camera->speed);
    fprintf(stdout, "--------------\n");
    fflush(stdout);
}

static void
input_process(GLFWwindow* window)
// TODO(eter): add HELD, RELEASED!
{
    // reset input state
    for (int i = 0; i < ACTION_COUNT; i++)
        g_input_state.actions[i] = false;

    for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; key++) {
        if (glfwGetKey(window, key) == GLFW_PRESS) {
            GameAction action = g_input_key_to_action[key];
            if (action != ACTION_NONE) {
                g_input_state.actions[action] = true;
            }
        }
    }
}

static void
camera_apply_movement(vec3 direction, double dt)
{
    vec3 final = {0};
    vec3_scale(final, direction, g_camera.speed * dt);
    vec3_add(g_camera.pos, g_camera.pos, final);
}

static void
camera_get_right_vector_norm(vec3 r)
{
    vec3_mul_cross(r, g_camera.direction, g_camera.up);
    vec3_norm(r, r);
}

static void
mini_update_camera_movement(double dt)
// NOTE(mdether7): Right now this function runs every frame. 
{
    vec3 movement_direction = {0};

    if (g_input_state.actions[ACTION_MOVE_FORWARD]) {
        vec3_add(movement_direction, movement_direction, g_camera.direction);
    }
    if (g_input_state.actions[ACTION_MOVE_BACKWARD]) {
        vec3_sub(movement_direction, movement_direction, g_camera.direction);
    }

    if (g_input_state.actions[ACTION_MOVE_LEFT] || g_input_state.actions[ACTION_MOVE_RIGHT]) {
        vec3 right;
        camera_get_right_vector_norm(right);

        if (g_input_state.actions[ACTION_MOVE_LEFT]) {
            vec3_sub(movement_direction, movement_direction, right);
        }
        if (g_input_state.actions[ACTION_MOVE_RIGHT]) {
            vec3_add(movement_direction, movement_direction, right);
        }
    }

    if (vec3_len(movement_direction) <= FLT_EPSILON) 
        return;

    vec3 normalized_movement = {0};
    vec3_norm(normalized_movement, movement_direction);
    camera_apply_movement(normalized_movement, dt);   
}

static void
camera_update_view_matrix(Camera* cam)
{
    vec3 center = {0};
    // we need to add to put the direction of the camera at
    vec3_add(center, cam->pos, cam->direction); // its current position
    mat4x4_look_at(cam->view, cam->pos, center, cam->up);
}

static void
camera_update_projection_matrix(Camera* cam)
{
    mat4x4_perspective(cam->projection,
                       mini_degrees_to_radians(cam->fov),
                       ((float)g_window_state.width / (float)g_window_state.height),
                       cam->near,
                       cam->far);
}

static void
mini_update_framecounter(FrameCounter* counter, double ms_per_frame)
{   
    counter->ms_per_frame = ms_per_frame;
    counter->avg_fps = 1000.0 / ms_per_frame;
}

static Direction
camera_get_facing_direction(Camera* cam)
// Doting with 2D vector ignoring Y.
{
    float dot_products[NWSE_COUNT];

    vec2 dir2d = { cam->direction[0], cam->direction[2] };
    dot_products[NORTH] = vec2_dot(dir2d, (vec2){DIR_FORWARD[0], DIR_FORWARD[2]});
    dot_products[EAST]  = vec2_dot(dir2d, (vec2){DIR_RIGHT[0], DIR_RIGHT[2]});
    dot_products[SOUTH] = vec2_dot(dir2d, (vec2){DIR_BACKWARD[0], DIR_BACKWARD[2]});
    dot_products[WEST]  = vec2_dot(dir2d, (vec2){DIR_LEFT[0], DIR_LEFT[2]}); 

    // assume it's the first one.
    Direction max_dir = NORTH;
    float max_val = dot_products[NORTH];

    for (size_t i = 1; i < NWSE_COUNT; i++)
        if (dot_products[i] > max_val) {
            max_val = dot_products[i];
            max_dir = (Direction)i;
        }

    return max_dir; 
}

//////////
// Utils
static const char*
util_get_direction_as_string(Direction dir)
{
    assert(dir >= NORTH && dir < NWSE_COUNT);
    static const char* names[] = {"NORTH", "EAST", "SOUTH", "WEST"};
    return names[dir];
}

///////////////////////////////////////////
//
//  GLFW callbacks
//
///////////////////////////////////////////

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{ 
    glViewport(0, 0, width, height);
    g_window_state.width   = width;
    g_window_state.height  = height;
    g_window_state.resized = true;
    // those width and height are the framebuffer sizes not window ones
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    //TODO: Move it to polling/while loop.
    static float xlast = 0;
    static float ylast = 0;
    static bool first_mouse = true;
    
    if (first_mouse) {
        xlast = xpos;
        ylast = ypos;
        first_mouse = false;
        return;
    }

    float xoffset = xpos - xlast; // calculate delta
    float yoffset = ylast - ypos; // same here, inverted!
    xlast = xpos;
    ylast = ypos;

    xoffset *= g_settings.mouse_sensitivity;
    yoffset *= g_settings.mouse_sensitivity;

    // g_camera.pitch = asin(-d.Y); // direction to pitch/yaw?
    // g_camera.yaw = atan2(d.X, d.Z)

    g_camera.yaw += xoffset;
    g_camera.pitch += yoffset;

    if(g_camera.pitch > 89.0f)
        g_camera.pitch = 89.0f;
    if(g_camera.pitch < -89.0f)
        g_camera.pitch = -89.0f;

    vec3 direction;
    direction[0] = cos(mini_degrees_to_radians(g_camera.yaw)) * cos(mini_degrees_to_radians(g_camera.pitch));
    direction[1] = sin(mini_degrees_to_radians(g_camera.pitch));
    direction[2] = sin(mini_degrees_to_radians(g_camera.yaw)) * cos(mini_degrees_to_radians(g_camera.pitch));
    vec3_norm(direction, direction);
    vec3_dup(g_camera.direction, direction);    
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Thats crazy useful! // 
    // int* my_int = (int*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
        shader_program_hot_reload(&g_shader_programs[PROGRAM_SLOT_0],
            "shaders/default.vert", "shaders/default.frag");
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
    if (g_user_config.should_start_focused) {
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    } 

#ifdef ENABLE_GL_DEBUG_OUTPUT
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); 
#endif

    /* create window */
    window = glfwCreateWindow(g_window_state.width, g_window_state.height, g_window_state.name, NULL, NULL);
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
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    /* Game/Engine specific input settings. (not sure if it is in good place tho)*/
    input_init_keybindings();
    // For motion based camera controls
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) // <= raw motion is better for controlling
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE); //  a 3D camera.

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
    g_shader_programs[PROGRAM_SLOT_0] = default_program; 

    // Triangle
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(mini_triangle), mini_triangle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint tri_color_VBO;
    glGenBuffers(1, &tri_color_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, tri_color_VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(mini_triangle_colors), mini_triangle_colors, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Cube
    GLuint cube_VAO;
    glGenVertexArrays(1, &cube_VAO);
    glBindVertexArray(cube_VAO);

    GLuint cube_VBO;
    glGenBuffers(1, &cube_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(mini_cube), mini_cube, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint color_VBO;
    glGenBuffers(1, &color_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, color_VBO);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)sizeof(mini_cube_colors), mini_cube_colors, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Get locations
    GLuint u_model_loc = glGetUniformLocation(g_shader_programs[PROGRAM_SLOT_0], "u_model");
    GLuint u_view_loc = glGetUniformLocation(g_shader_programs[PROGRAM_SLOT_0], "u_view");
    GLuint u_projection_loc = glGetUniformLocation(g_shader_programs[PROGRAM_SLOT_0], "u_projection");

    /*--------------------------------------------------------------------*/

    mat4x4 model_tri, model_cube;
    mat4x4_identity(model_tri);
    mat4x4_identity(model_cube);

    mat4x4_translate_in_place(model_tri, 0.0f, 0.5f, 0.0f);
    mat4x4_rotate_Y(model_tri, model_tri, mini_degrees_to_radians(120.0f));
    mat4x4_scale_aniso(model_tri, model_tri, 0.75f, 0.75f, 0.75f);

    mat4x4_translate_in_place(model_cube, 1.0f, 0.0f, 0.0f);
    mat4x4_rotate_Y(model_cube, model_cube, mini_degrees_to_radians(45.0f));
    mat4x4_scale_aniso(model_cube, model_cube, 0.75f, 0.75f, 0.75f);

    /* OpenGL initial options setup */
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); // <= read on this

    /* Game/Engine specific initialization */
    // Projection needs to be updated at least once before start
    camera_update_projection_matrix(&g_camera);

    glfwSetTime(0.0);
    float last_time = glfwGetTime();
    float previous_time = glfwGetTime();
    unsigned int frames = 0;
    float delta_time = 0.0; // TODO: Do something else than delta time (Ticks?)

    while (!glfwWindowShouldClose(window))
    { /*/.LOOP*/
        /* Start frame */
        float current_time = glfwGetTime();
        delta_time = current_time - previous_time;
        previous_time = current_time;

        /* Input */
        input_process(window);

        /* Update */
        mini_update_camera_movement(delta_time);

        // Update Matrices
        camera_update_view_matrix(&g_camera);

        if (g_window_state.resized) {
            camera_update_projection_matrix(&g_camera);
            mini_print_n_flush("W: %d, H: %d", g_window_state.width,
                                               g_window_state.height);
            g_window_state.resized = false;
        }

#if 0
        {
            Direction dir = camera_get_facing_direction(&g_camera);
            printf("[FACING:%s]\n", util_get_direction_as_string(dir));
        }
#endif
        /* Render */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(g_shader_programs[PROGRAM_SLOT_0]);

        // upload view and projection matrix (camera)
        glUniformMatrix4fv(u_view_loc, 1, GL_FALSE, &g_camera.view[0][0]);
        glUniformMatrix4fv(u_projection_loc, 1, GL_FALSE, &g_camera.projection[0][0]);

        // set triangle model
        glUniformMatrix4fv(u_model_loc, 1, GL_FALSE, &model_tri[0][0]);
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // set cube model
        glUniformMatrix4fv(u_model_loc, 1, GL_FALSE, &model_cube[0][0]);
        glBindVertexArray(cube_VAO); 
        glDrawArrays(GL_TRIANGLES, 0, 36);
       
        /* Present frame */
        glfwSwapBuffers(window);
        glfwPollEvents();

        /* Frame Counter */
        frames++;
        if ( current_time - last_time >= 1.0) {
            mini_update_framecounter(&g_frame_counter, 1000.0f/(float)frames);
            mini_print_n_flush("[FPS COUNTER: %.2f ms/frame | %.2f FPS]", 
               g_frame_counter.ms_per_frame, g_frame_counter.avg_fps);
            frames = 0;
            last_time += 1.0;
            mini_print_camera(&g_camera); // DEBUG ONLY
        }
    }

    /* OpenGL objects cleanup */
    glDeleteProgram(g_shader_programs[PROGRAM_SLOT_0]);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &cube_VAO);
    glDeleteBuffers(1, &cube_VBO);
    glDeleteBuffers(1, &color_VBO);

    // no need to destroy technically.
    glfwDestroyWindow(window);

    glfwTerminate();
    return EXIT_SUCCESS;
}
