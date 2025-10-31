#ifndef PLATFORM_TOOLS_H
#define PLATFORM_TOOLS_H

#include <stdlib.h>
#include <stdio.h>

#define RELEASE_ASSERT(x) \
    do { \
        if (!x) { \
            fprintf(stderr, "Release assertion failed: %s, file %s, line %d\n", \
            #x, __FILE__, __LINE__); \
            abort(); \
        } \
    } while(0)

#endif 