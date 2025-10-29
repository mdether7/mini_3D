#ifndef PLATFORM_GLFW_H
#define PLATFORM_GLFW_H

int platform_initialize(const char* window_name, int w, int h);
int platform_window_should_close(void);
void platform_present_frame(void);

#endif