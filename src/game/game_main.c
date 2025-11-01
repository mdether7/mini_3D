#include "game_main.h"

#include <glad/glad.h>

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
    game_state.point_size += 1.0f;
    glPointSize(game_state.point_size);
    if (game_state.point_size >= 200.0f) {
        game_state.point_size = 1.0f;
    }

    // render.
    glBindVertexArray(game_state.vao);
    shader_program_bind(&game_state.shady);
    glDrawArrays(GL_POINTS, 0, 2);

    return 0;
}

void dg_close(void)
{
    shader_program_delete(&game_state.shady);
    glDeleteVertexArrays(1, &game_state.vao);
}