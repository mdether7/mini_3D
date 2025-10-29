#ifndef PLATFORM_LOG_H
#define PLATFORM_LOG_H

void platform_log_error(const char* fmt, ...);
void platform_log_info(const char* fmt, ...);
void platform_log_success(const char* fmt, ...);

#endif