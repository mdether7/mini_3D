#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __linux__
#define TERMINAL_RED "\e[1;31m"
#define TERMINAL_GREEN "\e[0;32m"
#define TERMINAL_CYAN "\e[0;36m"
#define TERMINAL_RESET "\e[0m"
#elif defined(_WIN32)
#define TERMINAL_RED "\e[1;31m"
#define TERMINAL_GREEN "\e[0;32m"
#define TERMINAL_CYAN "\e[0;36m"
#define TERMINAL_RESET "\e[0m"
#else 
#define TERMINAL_RED ""
#define TERMINAL_GREEN ""
#define TERMINAL_CYAN ""
#define TERMINAL_RESET ""
#endif

//TODO: Add windows support for colorful output.

void platform_log_error(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    
    fprintf(stderr, TERMINAL_RED "[ERROR]: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, TERMINAL_RESET "\n");
    va_end(ap);
    fflush(stderr);
}

void platform_log_notify(char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stdout, TERMINAL_CYAN "[NOTIFY]: ");
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, TERMINAL_RESET "\n");
    va_end(ap);
    fflush(stdout);
}


// void
// util_print_n_flush(char* fmt, ...)
// {
//     va_list ap;
//     va_start(ap, fmt);
//     vfprintf(stdout, fmt, ap);
//     va_end(ap);
//     fputs("\n", stdout);
//     fflush(stdout);
// }