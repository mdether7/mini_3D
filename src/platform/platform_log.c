#include "platform_log.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef __linux__
    #define TERMINAL_RED "\e[1;31m"
    #define TERMINAL_GREEN "\e[0;32m"
    #define TERMINAL_CYAN "\e[0;36m"
    #define TERMINAL_RESET "\e[0m"
#elif defined(_WIN32) //TODO: Add windows support for colorful output.
    #define TERMINAL_RED ""
    #define TERMINAL_GREEN ""
    #define TERMINAL_CYAN ""
    #define TERMINAL_RESET ""
#else 
    #define TERMINAL_RED ""
    #define TERMINAL_GREEN ""
    #define TERMINAL_CYAN ""
    #define TERMINAL_RESET ""
#endif

void platform_log_error(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, TERMINAL_RED "[ERROR]: ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, TERMINAL_RESET "\n");
    va_end(ap);
    fflush(stderr);
}

void platform_log_info(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stdout, TERMINAL_CYAN "[INFO]: ");
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, TERMINAL_RESET "\n");
    va_end(ap);
    fflush(stdout);
}

void platform_log_success(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stdout, TERMINAL_GREEN "[SUCCESS]: ");
    vfprintf(stdout, fmt, ap);
    fprintf(stdout, TERMINAL_RESET "\n");
    va_end(ap);
    fflush(stdout);
}

