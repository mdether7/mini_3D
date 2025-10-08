#include "math_debug.h"

#include <stdio.h>
#include <linmath/linmath.h>

void mini_math_print_mat4x4(const mat4x4 M)
{
    int i, j;
	for(i=0; i<4; ++i) {
        for(j=0; j<4; ++j) {
            fprintf(stdout, "%.2f ", M[i][j]);
        }
        putchar('\n');
    }
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