#ifndef MINI_MATH_DEBUG_H
#define MINI_MATH_DEBUG_H

#include <linmath/linmath.h>

#define MINI_PI 3.14159265358979323846f

#define MAT4x4_IDENTITY {{1.0f, 0.0f, 0.0f, 0.0f}, \
                         {0.0f, 1.0f, 0.0f, 0.0f}, \
                         {0.0f, 0.0f, 1.0f, 0.0f}, \
                         {0.0f, 0.0f, 0.0f, 1.0f}} \

/////////////////////////////
// Debug prints for linmath
void mini_math_print_mat4x4(const mat4x4 M);
void mini_math_print_vec4(const vec4 V);
void mini_math_print_vec3(const vec3 V);
void mini_math_print_vec2(const vec2 V);

////////////
// My math
////////////

////////////////
// Conversions
float mini_degrees_to_radians(float d);
float mini_radians_to_degrees(float r);

////////////
// Vectors
float vec3_dot(vec3 v1, vec3 v2);
float vec2_dot(vec2 v1, vec2 v2);

////////////////
// Comparisons
// (now i know there's math.h functions for this XD)s
float minf(float f1, float f2);
float maxf(float f1, float f2);

#endif