#include "math_helpers.h"

#include <stdio.h>
#include <linmath/linmath.h>

/////////////////////////////
// Debug prints for linmath
void mini_math_print_mat4x4(const mat4x4 M)
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

void mini_math_print_vec4(const vec4 V)
{
    fprintf(stdout, "%.2f, %.2f, %.2f, %.2f\n", V[0], V[1], V[2], V[3]);
    fflush(stdout);
}

void mini_math_print_vec3(const vec3 V)
{
    fprintf(stdout, "%.2f, %.2f, %.2f\n", V[0], V[1], V[2]);
    fflush(stdout);
}

void mini_math_print_vec2(const vec2 V)
{
    fprintf(stdout, "%.2f, %.2f\n", V[0], V[1]);
    fflush(stdout);
}

////////////////
// Conversions
float mini_degrees_to_radians(float d)
{
    return (MINI_PI / 180.0f) * d;
}

float mini_radians_to_degrees(float r)
{
    return (180.0f / MINI_PI) * r;
}

////////////
// My math
float vec3_dot(vec3 v1, vec3 v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}