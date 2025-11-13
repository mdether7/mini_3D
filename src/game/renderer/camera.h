#ifndef DG_CAMERA_H
#define DG_CAMERA_H

#include <linmath/linmath.h>

typedef struct {
    mat4x4 projection;
    mat4x4 view;
    float near;
    float far;
    float fov;
} DG3D_Camera;

#endif