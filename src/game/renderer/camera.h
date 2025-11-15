#ifndef DG_CAMERA_H
#define DG_CAMERA_H

#include <linmath/linmath.h>

typedef struct {
    mat4x4 view;
    mat4x4 projection;
    vec3  pos;
    vec3  target;
    vec3  up;
    float fov;
    float aspect_ratio;
    float znear; 
    float zfar; 

    // euler Angles
    float yaw;
    float pitch;

    // Movement
    vec3  mov_velocity;
    float mov_speed;
    float mov_friction;
} DG3D_Camera;

int camera_init(DG3D_Camera* cam, const vec3 pos, const vec3 target, const vec3 up, 
    float fov, float width, float height, float znear, float zfar);

void camera_update(DG3D_Camera* cam, float dt);
void camera_update_on_screen_resize(DG3D_Camera* cam, float width, float height);
void camera_set_fov(DG3D_Camera* cam, float fov);
void camera_set_clip_planes(DG3D_Camera* cam, float znear, float zfar);
void camera_get_projection_matrix(DG3D_Camera* cam, mat4x4 out);
void camera_get_view_matrix(DG3D_Camera* cam, mat4x4 out);

// void camera_rotate(DG3D_Camera* cam, float yaw, float pitch);
// void camera_move(DG3D_Camera* cam, vec3 direction);
// void camera_set_position(DG3D_Camera* cam, const vec3 pos);
// void camera_look_at(DG3D_Camera* cam, const vec3 target);

#endif