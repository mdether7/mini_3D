#include "game_main.h"

#include <glad/glad.h>
#include <math.h>

#include "platform/platform_input.h"
#include "platform/platform_log.h"
#include "renderer/shader.h"


// https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value
static inline float clampf(float f, float min, float max) {
  const float t = f < min ? min : f;
  return t > max ? max : t;
}

typedef struct {
    Shader shady;
    Shader tess_shady;
    GLuint vao;
    float  point_size;
} GameState;

static GameState game_state = {0}; // static?

int dg_init(void)
{
    // game_state.shady = shader_program_compile_from_path("shaders/dungen.vert", "shaders/dungen.frag");
    // if (game_state.shady.id == 0) {
    //     return 1;
    // }

    game_state.tess_shady = shader_program_tess_compile_from_path("shaders/dungen.vert",
        "shaders/dungen.tcs", "shaders/dungen.tes", "shaders/dungen.frag");
    if (game_state.tess_shady.id == 0) {
        return 1;
    }

    glGenVertexArrays(1, &game_state.vao);

    game_state.point_size = 1.0f;

    return 0;
}

float red;
float green;
float blue;
int dg_loop(float dt)
{
    // input.
    if (platform_is_key_down(KEY_W)) {
        red = green = blue = 0.0f;
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
    if (platform_is_key_down(KEY_R)) {
        red += 0.05f;
        red = clampf(red, 0.0f, 1.0f);
    }
    if (platform_is_key_down(KEY_G)) {
        green += 0.05f;
        green = clampf(green, 0.0f, 1.0f);
    }
    if (platform_is_key_down(KEY_B)) {
        blue += 0.05f;
        blue = clampf(blue, 0.0f, 1.0f);
    }

    // update.
    GLfloat attrib[] = { (float)sin(dt) * 0.5f, 
        (float)cos(dt) * 0.6f, 0.0f, 0.0f };
    GLfloat color[] = {red, green, blue, 1.0f};

    glVertexAttrib2fv(0, attrib);
    glVertexAttrib4fv(1, color);
    
    // TODO: Test if glVertexAttribPointer() will change with me
    //       updating the array being pointed at.

    // render.
    platform_log_info("%f ,%f, %f", red, green, blue);
    glClearBufferfv(GL_COLOR, 0, (GLfloat[]){0.0f, 0.0f, 0.0f, 1.0f});

    // shader_program_bind(&game_state.shady);
    // glBindVertexArray(game_state.vao);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    return 0;
}

void dg_close(void)
{
    shader_program_delete(&game_state.shady);
    glDeleteVertexArrays(1, &game_state.vao);
}