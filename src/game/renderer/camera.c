#include "camera.h"

#include <assert.h>
#include <linmath/linmath.h>

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

    return 0;
}

void camera_get_projection_matrix(DG3D_Camera* cam, mat4x4 out)
{
    mat4x4_dup(cam->projection, out);
}

void camera_get_view_matrix(DG3D_Camera* cam, mat4x4 out)
{
    mat4x4_dup(cam->view, out);
}