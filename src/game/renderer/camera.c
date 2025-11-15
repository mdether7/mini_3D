#include "camera.h"

#include <assert.h>
#include <float.h>
#include <linmath/linmath.h>

#include "platform/platform_input.h"

#include "platform/platform_log.h"
#include <stdio.h>

#if 0
void mathy_print_mat4x4(const mat4x4 M)
{
    int i, j;
    fprintf(stdout, "-------\n");
	for(i=0; i<4; ++i) {
        for(j=0; j<4; ++j) {
            fprintf(stdout, "%.2f ", M[j][i]);
        }
        putchar('\n');
    }
    fprintf(stdout, "-------\n");
    fflush(stdout);
}
#endif

int camera_init(DG3D_Camera* cam, const vec3 pos, const vec3 target, const vec3 up, float fov, float width, float height, float znear, float zfar) 
{
    mat4x4_identity(cam->view);
    mat4x4_identity(cam->projection);

    cam->pos[0] = pos[0];
    cam->pos[1] = pos[1];
    cam->pos[2] = pos[2];

    cam->target[0] = target[0];
    cam->target[1] = target[1];
    cam->target[2] = target[2];

    cam->up[0] = up[0];
    cam->up[1] = up[1];
    cam->up[2] = up[2];

    float aspect_ratio = width / height;

    mat4x4_look_at(cam->view, pos, target, up);
    mat4x4_perspective(cam->projection, fov, aspect_ratio, znear, zfar);

    // Movement aspects.
    cam->mov_speed = 5.0f;
    cam->mov_friction = 0.95f;
    cam->mov_velocity[0] = 0.0f;
    cam->mov_velocity[1] = 0.0f;
    cam->mov_velocity[2] = 0.0f;

    return 0;
}

void camera_update(DG3D_Camera* cam, float dt)
{
    vec3 direction = {0};
    if (platform_is_key_down(KEY_W)) {
        vec3_sub(direction, direction, (vec3){0.0f, 0.0f, 1.0f});
    }   
    if (platform_is_key_down(KEY_S)) {
        vec3_add(direction, direction, (vec3){0.0f, 0.0f, 1.0f});
    }
    if (platform_is_key_down(KEY_A)) {
        vec3_sub(direction, direction, (vec3){1.0f, 0.0f, 0.0f});
    }
    if (platform_is_key_down(KEY_D)) {
        vec3_add(direction, direction, (vec3){1.0f, 0.0f, 0.0f});
    }

    if (vec3_len(direction) > FLT_EPSILON) {
        vec3_norm(direction, direction);
        vec3_scale(cam->mov_velocity, direction, cam->mov_speed);
    }

    vec3 displacement;
    vec3_scale(displacement, cam->mov_velocity, dt);
    vec3_add(cam->pos, cam->pos, displacement);

    // apply friction
    vec3_scale(cam->mov_velocity, cam->mov_velocity, cam->mov_friction);

    if (vec3_len(cam->mov_velocity) <= FLT_EPSILON) {
        cam->mov_velocity[0] = 0.0f;
        cam->mov_velocity[1] = 0.0f;
        cam->mov_velocity[2] = 0.0f;
    }

    // update cam view matrix;
    vec3 target;
    vec3_sub(target, cam->pos, (vec3){0.0f, 0.0f, 1.0f});
    mat4x4_look_at(cam->view, cam->pos, target, cam->up);
}

void camera_update_on_screen_resize(DG3D_Camera* cam, float width, float height)
{
    float aspect_ratio = width / height;
    mat4x4_perspective(cam->projection, cam->fov, aspect_ratio, cam->znear, cam->zfar);
}

// in radians!!
void camera_set_fov(DG3D_Camera* cam, float fov)
{
    cam->fov = fov;
}

void camera_set_clip_planes(DG3D_Camera* cam, float znear, float zfar)
{
    cam->znear = znear;
    cam->zfar = zfar;
}  

void camera_get_projection_matrix(DG3D_Camera* cam, mat4x4 out)
{
    mat4x4_dup(cam->projection, out);
}

void camera_get_view_matrix(DG3D_Camera* cam, mat4x4 out)
{
    mat4x4_dup(cam->view, out);
}