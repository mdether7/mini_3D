#ifndef MINI_UTILS_H
#define MINI_UTILS_H

#define TERMINAL_RED "\e[1;31m"
#define TERMINAL_GREEN "\e[0;32m"
#define TERMINAL_RESET "\e[0m"

//////////////////////////
// Mini specific utility
// and helper functions
void mini_die(char* fmt, ...); // <- TODO: move this to mini_3D.c
void mini_print_n_flush(char* fmt, ...); // TODO: rename this to util_print_n...

////////////////////////////////
// BMP image loader TODO?
// (For learning purposes only)



#endif