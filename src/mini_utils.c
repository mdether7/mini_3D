#include "mini_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "vertex_type.h"

void
mini_die(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fputs("\n", stderr);
    exit(EXIT_FAILURE);
}

void
util_print_n_flush(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
    fputs("\n", stdout);
    fflush(stdout);
}

void 
util_print(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);
    va_end(ap);
}

void 
util_print_vertex3d(Vertex3D vert)
{
    printf("=VERTEX=\n");
    printf("POS: %f, %f, %f\n", vert.position[0], vert.position[1], vert.position[2]);
    printf("========\n");
   // printf("NORM: %f, %f, %f\n", vert.normal[0], vert.normal[1], vert.normal[2]);
   // printf("UV: %f, %f\n", vert.uv[0], vert.uv[1]);
}
