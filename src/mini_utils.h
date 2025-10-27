#ifndef MINI_UTILS_H
#define MINI_UTILS_H

#include "vertex_type.h"
#include "mini_types.h"

////////////////////
// Terminal Colors
#define TERMINAL_RED "\e[1;31m"
#define TERMINAL_GREEN "\e[0;32m"
#define TERMINAL_CYAN "\e[0;36m"
#define TERMINAL_RESET "\e[0m"

//////////////////////////
// Mini specific utility
// and helper functions
void mini_die(char* fmt, ...); // <- TODO: move this to mini_3D.c

void util_print_n_flush(char* fmt, ...);
void util_print(char* fmt, ...);

void util_print_vertex3d(Vertex3D vert);

// NOTES: unbinds currently bound texture, 
// caller need's to rebind if is using it.
int util_texture_save_to_disk_as_png(TextureID texture, const char* name);



////////////////////////////////
// BMP image loader TODO?
// (For learning purposes only)



#endif