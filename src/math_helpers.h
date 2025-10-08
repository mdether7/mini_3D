#ifndef MINI_MATH_DEBUG_H
#define MINI_MATH_DEBUG_H

#include <linmath/linmath.h>

#define MINI_PI 3.14159265358979323846f

/////////////////////////////
// Debug prints for linmath
void mini_math_print_mat4x4(const mat4x4 M);
void mini_math_print_vec4(const vec4 V);
void mini_math_print_vec3(const vec3 V);
void mini_math_print_vec2(const vec2 V);

////////////////
// Conversions
float mini_degrees_to_radians(float d);
float mini_radians_to_degrees(float r);

#endif