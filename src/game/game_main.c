#include "game_main.h"

#include <glad/glad.h>
#include <math.h>

#include "../../gleter2d/gleter2d.h" // placeholder, this needs to be a lib.
#include "platform/platform_input.h"
#include "platform/platform_other.h"
#include "platform/platform_log.h"
#include "renderer/shader.h"


// https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value
static inline float clampf(float f, float min, float max) {
  const float t = f < min ? min : f;
  return t > max ? max : t;
}

typedef struct {
    GLE2D_Font default_font;
    GLE2D_Font extra_font;
} DG_Fonts;

typedef struct {
    DG_Fonts fonts;
    GLE2D_Texture dirt_tex;
    Shader tess_shady;
    GLuint vao;
} DG_GameState;

static DG_GameState game_state = {0};

#define FULL_WHITE (vec4){1.0f, 1.0f, 1.0f, 1.0f}
#define TEXT_COLOR (vec4){0.0f, 0.5f, 0.5f, 1.0f}

int dg_init(void)
{
    if (gle2d_init()) {
        return 1;
    }
    // Usually you want viewport to match framebuffer
    int w, h;
    platform_get_framebuffer_size(&w, &h);
    glViewport(0, 0, w, h); 
    gle2d_update_rendering_area(w, h);

    game_state.tess_shady = shader_program_tess_compile_from_path("shaders/dungen.vert",
        "shaders/dungen.tcs", "shaders/dungen.tes", "shaders/dungen.frag");
    if (game_state.tess_shady.id == 0) {
        return 1;
    }

    const char* path = "fonts/ligurino_bold.ttf";
    if (gle2d_font_create(&game_state.fonts.default_font, path, 16.0f)) {
        return 1;
    }
    const char* path_2 = "fonts/ligurino.ttf";
    if (gle2d_font_create(&game_state.fonts.extra_font, path_2, 64.0f)) {
        return 1;
    }

    const char* path_3 = "textures/mini_dirt.png";
    game_state.dirt_tex = gle2d_texture_load(path_3);
    if (game_state.dirt_tex.id == 0) {
        return 1;
    }

    // GLint currentFBO, readFBO;
    // glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &currentFBO);
    // glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFBO);
    // platform_log_info("%d, %d", currentFBO, readFBO);

    glGenVertexArrays(1, &game_state.vao);
    return 0;
}

int point_x = 100;
int point_y = 100;

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

    // TODO Test this out.
    // GLint prev_viewport[4];
    // glGetIntegerv(GL_VIEWPORT, prev_viewport);

    // update.
    GLfloat attrib[] = {(float)sin(dt) * 0.5f, (float)cos(dt) * 0.6f};
    GLfloat color[] = {1.0f, 0.0f, 0.5f, 1.0f};
    glGetAttribLocation(game_state.tess_shady.id, "offset");
    glGetAttribLocation(game_state.tess_shady.id, "color");
    glVertexAttrib2fv(0, attrib);
    glVertexAttrib4fv(1, color);

    // render.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearBufferfv(GL_COLOR, 0, (GLfloat[]){0.0f, 0.0f, 0.0f, 1.0f});
    shader_program_bind(&game_state.tess_shady);
    glBindVertexArray(game_state.vao);
    glDrawArrays(GL_PATCHES, 0, 6);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    const char* text = "Lorem ipsum";
    gle2d_font_render_text(&game_state.fonts.default_font, TEXT_COLOR, text, 50, 50);
    gle2d_font_render_text(&game_state.fonts.extra_font, (vec4){0.0f, 0.5f, 0.5f, (float)sin(dt)}, text, 50, 100);
    gle2d_font_render_text(&game_state.fonts.default_font, TEXT_COLOR, text, 50, 150);
    gle2d_font_render_text(&game_state.fonts.extra_font, TEXT_COLOR, text, 50, 200);
    gle2d_shapes_draw_quad(500, 250, 256, 200, ((float)sin(dt) * 1.0f), FULL_WHITE, game_state.dirt_tex.id);

    //256.000000, 14.000000

    gle2d_font_render_text_rotation(&game_state.fonts.default_font, "HEHE SPIN MEEEEEEEEEEEEEEEEEEEE", 300, 300, (float)sin(dt) * 0.5f, TEXT_COLOR);

    gle2d_shapes_draw_glpoint(300, 300, 20, TEXT_COLOR);

    return 0;
}

void dg_close(void)
{
    shader_program_delete(&game_state.tess_shady);
    glDeleteVertexArrays(1, &game_state.vao);

    gle2d_font_destroy(&game_state.fonts.default_font);
    gle2d_font_destroy(&game_state.fonts.extra_font);
    gle2d_texture_delete(game_state.dirt_tex);
    gle2d_shutdown();
}