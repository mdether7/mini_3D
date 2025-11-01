#include "game_main.h"

#include <glad/glad.h>
#include <math.h>

#include "platform/platform_input.h"
#include "platform/platform_log.h"
#include "renderer/shader.h"

typedef struct {
    Shader shady;
    GLuint vao;
    float  point_size;
} GameState;

static GameState game_state = {0}; // static?

int dg_init(void)
{
    game_state.shady = shader_program_compile_from_path("shaders/dungen.vert", "shaders/dungen.frag");
    if (game_state.shady.id == 0) {
        return 1;
    }
    glGenVertexArrays(1, &game_state.vao);

    game_state.point_size = 1.0f;

    return 0;
}

int dg_loop(float dt)
{
    // input.
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

    // update.
    GLfloat attrib[] = { (float)sin(dt) * 0.5f, 
        (float)cos(dt) * 0.6f, 0.0f, 0.0f };

    glVertexAttrib2fv(0, attrib);

    // render.
    glClearBufferfv(GL_COLOR, 0, (GLfloat[]){0.0f, 0.0f, 0.0f, 1.0f});

    glBindVertexArray(game_state.vao);
    shader_program_bind(&game_state.shady);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    return 0;
}

void dg_close(void)
{
    shader_program_delete(&game_state.shady);
    glDeleteVertexArrays(1, &game_state.vao);
}