#define GLFW_INCLUDE_NONE // <- glad/GLFW can be included in any order
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "platform_log.h"


#define UNUSED(var) (void)var

static void error_callback(int error, const char* description)
{
    // fprintf(stderr, "Error: %s\n", description);
    platform_log_error()
}





