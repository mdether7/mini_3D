#include "platform/platform_glfw.h"

int main(void)
{
    int w = 1280;
    int h = 720;

    if (platform_initialize("DunGeN", w, h)) {
        exit(1);
    }

    while (!platform_window_should_close())
    {



        platform_present_frame();
    }



    




}