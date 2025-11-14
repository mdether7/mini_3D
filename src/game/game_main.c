#include "game_main.h"

#include <stdint.h>
#include <math.h>

#include <stdio.h> // DEBUG ONLY.

#include <glad/glad.h>

#include "../../gleter2d/gleter2d.h" // placeholder, this needs to be a lib.
#include "platform/platform_input.h"
#include "platform/platform_other.h"
#include "platform/platform_log.h"
#include "renderer/camera.h"
#include "renderer/renderer.h"
#include "renderer/shader.h"

#define TEXT_COLOR (vec4){0.0f, 0.5f, 0.5f, 1.0f}

#if 0
// https://stackoverflow.com/questions/427477/fastest-way-to-clamp-a-real-fixed-floating-point-value
static inline float clampf(float f, float min, float max) {
  const float t = f < min ? min : f;
  return t > max ? max : t;
}
#endif

typedef struct {
    GLE2D_Font default_font;
    GLE2D_Font extra_font;
} DG_Fonts;

typedef struct {
    DG_Fonts fonts;
    GLE2D_Texture dirt_tex;
    GLuint tess_shady;
    DG3D_Renderer renderer;
    DG3D_Camera camera;
    GLuint vao;
} DG_GameState;
static DG_GameState game_state = {0};

int fb_w, fb_h;

int dg_init(void)
{
    platform_get_framebuffer_size(&fb_w, &fb_h);

    if (gle2d_init()) return 1;
    gle2d_update_rendering_area(fb_w, fb_h);

    if (dg3d_renderer_init(&game_state.renderer, fb_w, fb_h)) {
        platform_log_error("Renderer init failed!");
        return 1;
    }

    vec3 pos = {0.0f, 0.0f, 5.0f};
    vec3 target = {0.0f, 0.0f, 0.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    camera_init(&game_state.camera, pos, target, up, 1.0f, (float)fb_w, (float)fb_h, 0.1f, 100.f);
    dg3d_renderer_set_camera(&game_state.renderer, &game_state.camera);

    game_state.tess_shady = shader_program_tess_compile_from_path("shaders/dungen.vert",
        "shaders/dungen.tcs", "shaders/dungen.tes", "shaders/dungen.frag");
    if (game_state.tess_shady == 0) {
        return 1;
    }

    const char* path = "fonts/ligurino_bold.ttf";
    if (gle2d_font_create(&game_state.fonts.default_font, path, 32.0f)) {
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

    platform_log_info("w:%d, h:%d", fb_w, fb_h);

    glGenVertexArrays(1, &game_state.vao);
    return 0;
}

vec3 velocity = {1.0f};
vec3 pos = {0};

int dg_loop(float dt)
{
    if (platform_is_key_down(KEY_R)) {
        if (gle2d_misc_shader_hot_reload(GLE2D_SHADER_SOLID, "shaders/gle2dsolid.vert", "shaders/gle2dsolid.frag")) {
            platform_log_error("Shader hot reaload error!");
        } else {
            platform_log_success("Shader reloaded sucesfully!");
        }
    }

    vec3_add(pos, pos, velocity);

    if (platform_is_key_down(KEY_A)) {
        fprintf(stdout, "%.2f, %.2f, %.2f\n", pos[0], pos[1], pos[2]);
        fflush(stdout);
    }


    // update.
    GLfloat attrib[] = {(float)sin(dt) * 0.5f, (float)cos(dt) * 0.6f};
    GLfloat color[] = {1.0f, 0.0f, 0.5f, 1.0f};
    glGetAttribLocation(game_state.tess_shady, "offset");
    glGetAttribLocation(game_state.tess_shady, "color");
    glVertexAttrib2fv(0, attrib);
    glVertexAttrib4fv(1, color);

    camera_update(&game_state.camera, 0.0f);
    
    gle2d_update_time_uniform(dt);

    // render.

    mat4x4 model_1;
    mat4x4 model_2;
    mat4x4 model_3;
    mat4x4 model_4;
    mat4x4 model_5;
    mat4x4 model_6;
    mat4x4 model_7;
    mat4x4 model_8;
    mat4x4_translate(model_1, (float)cos(dt), (float)sin(dt), -5.0f);
    mat4x4_translate(model_2, 0.0f, 5.0f, -5.0f);
    mat4x4_translate(model_3, 1.0f, 0.0f, -5.0f);
    mat4x4_translate(model_4, 0.0f, 3.0f, -5.0f);
    mat4x4_translate(model_5, 3.0f, 0.0f, -5.0f);
    mat4x4_translate(model_6, 2.0f, 5.0f, -5.0f);
    mat4x4_translate(model_7, 0.0f, 6.0f, -5.0f);
    mat4x4_translate(model_8, 5.0f, 20.0f, -5.0f);
    dg3d_begin_frame(&game_state.renderer);
    dg3d_render_cube(&game_state.renderer, model_1, game_state.dirt_tex.id);
    dg3d_render_cube(&game_state.renderer, model_2, game_state.dirt_tex.id);
    dg3d_render_cube(&game_state.renderer, model_3, game_state.dirt_tex.id);
    dg3d_render_cube(&game_state.renderer, model_4, game_state.dirt_tex.id);
    dg3d_render_cube(&game_state.renderer, model_5, game_state.dirt_tex.id);
    dg3d_render_cube(&game_state.renderer, model_6, game_state.dirt_tex.id);
    dg3d_render_cube(&game_state.renderer, model_7, game_state.dirt_tex.id);
    dg3d_render_cube(&game_state.renderer, model_8, game_state.dirt_tex.id);


    // glClearBufferfv(GL_COLOR, 0, (GLfloat[]){0.0f, 0.0f, 0.0f, 1.0f});
    //gle2d_shapes_draw_quad(0, 0, fb_w, fb_h, 0.0f, GLE2D_COLOR_GREEN, 0);

#if 1 // MY 2D LIB SHOWCASE
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shader_program_bind(game_state.tess_shady);
    glBindVertexArray(game_state.vao);
    glDrawArrays(GL_PATCHES, 0, 6);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    const char* text = "gleter2d";
    gle2d_font_render_text(&game_state.fonts.default_font, TEXT_COLOR, text, 0, 0);
    gle2d_font_render_text(&game_state.fonts.extra_font, (vec4){0.0f, 0.5f, 0.5f, (float)sin(dt)}, text, 50, 100);
    gle2d_font_render_text(&game_state.fonts.default_font, TEXT_COLOR, text, 50, 150);
    gle2d_font_render_text(&game_state.fonts.extra_font, TEXT_COLOR, text, 50, 200);
    gle2d_shapes_draw_quad(fb_w / 3, fb_h / 3, 256 *(float)sin(dt) , 200 * (float)cos(dt), ((float)sin(dt) * 1.0f), GLE2D_COLOR_WHITE, game_state.dirt_tex.id);
    gle2d_font_render_text_rotation(&game_state.fonts.default_font, "gleter2d rotatin text", 200.0f, 200.0f, (float)sin(dt), TEXT_COLOR);
    gle2d_font_render_text(&game_state.fonts.default_font, GLE2D_COLOR_WHITE, "0,0", 0, 680);
    gle2d_font_render_text(&game_state.fonts.default_font, GLE2D_COLOR_WHITE, "1,0", 1220, 680);
    gle2d_font_render_text(&game_state.fonts.default_font, GLE2D_COLOR_WHITE, "1,1", 1220, 0);
    gle2d_font_render_text(&game_state.fonts.default_font, GLE2D_COLOR_WHITE, "0,1", 0, 0);
#endif

    return 0;
}

void dg_close(void)
{
    shader_program_delete(game_state.tess_shady);
    glDeleteVertexArrays(1, &game_state.vao);

    dg3d_renderer_shutdown(&game_state.renderer);

    gle2d_font_destroy(&game_state.fonts.default_font);
    gle2d_font_destroy(&game_state.fonts.extra_font);
    gle2d_texture_delete(game_state.dirt_tex);
    gle2d_shutdown();
}
