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
    Shader tess_shady;
    GLuint vao;
} DG_GameState;

static DG_GameState game_state = {0}; // static?

int dg_init(void)
{
    if (gle2d_init()) {
        return 1;
    }

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

    // Usually you want viewport to match framebuffer
    int dims[2];
    platform_get_framebuffer_size(dims);
    glViewport(0, 0, dims[0], dims[1]); 
    gle2d_update_rendering_area(dims[0], dims[1]);
    
    glGenVertexArrays(1, &game_state.vao);
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
    GLfloat attrib[] = {(float)sin(dt) * 0.5f, (float)cos(dt) * 0.6f};
    GLfloat color[] = {1.0f, 0.0f, 0.5f, 1.0f};

    platform_log_info("%d, %d", 
    glGetAttribLocation(game_state.tess_shady.id, "offset"),
    glGetAttribLocation(game_state.tess_shady.id, "color"));

    glVertexAttrib2fv(0, attrib);
    glVertexAttrib4fv(1, color);
    
    // TODO: Test if glVertexAttribPointer() will change when
    // TODO  updating the array being pointed at.

    // render.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearBufferfv(GL_COLOR, 0, (GLfloat[]){0.0f, 0.0f, 0.0f, 1.0f});
    shader_program_bind(&game_state.tess_shady);
    glBindVertexArray(game_state.vao);
    glDrawArrays(GL_PATCHES, 0, 6);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    const char* text = "Lorem ipsum";
    gle2d_font_render_text(&game_state.fonts.default_font, text, 50, 50);
    gle2d_font_render_text(&game_state.fonts.extra_font, text, 50, 100);
    gle2d_font_render_text(&game_state.fonts.default_font, text, 50, 150);
    gle2d_font_render_text(&game_state.fonts.extra_font, text, 50, 200);
    
    gle2d_shapes_draw_quad(300, 200, 200, 200, (vec4){1.0f, 0.3f, 0.5f, 0.4f}, 0);

    return 0;
}

void dg_close(void)
{
    shader_program_delete(&game_state.tess_shady);
    glDeleteVertexArrays(1, &game_state.vao);

    gle2d_font_destroy(&game_state.fonts.default_font);
    gle2d_font_destroy(&game_state.fonts.extra_font);
    gle2d_shutdown();
}