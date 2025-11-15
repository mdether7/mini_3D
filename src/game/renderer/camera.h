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
    float znear; 
    float zfar; 

    // Movement
    vec3  mov_velocity;
    float mov_speed;
    float mov_friction;

} DG3D_Camera;

int camera_init(DG3D_Camera* cam, const vec3 pos, const vec3 target, const vec3 up, 
    float fov, float width, float height, float znear, float zfar);

void camera_print_movement(DG3D_Camera* cam);

void camera_update(DG3D_Camera* cam, float delta_time);
void camera_get_projection_matrix(DG3D_Camera* cam, mat4x4 out);
void camera_get_view_matrix(DG3D_Camera* cam, mat4x4 out);

#endif