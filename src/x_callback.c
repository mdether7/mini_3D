///////////////////////////////////////////////////////
//
//   Tiny tiny callback showcase for my own sanity...
//
///////////////////////////////////////////////////////

#include <stdio.h>
#include <stddef.h>

struct call_me {

    void (*on_error)(const char*);

};

struct call_me g_call_me_baybe = {
    .on_error = NULL,
};

static void
str_call_on_error(void)
{
    if (g_call_me_baybe.on_error)
        g_call_me_baybe.on_error("HEHE");
}

static void
set_callback_function(void (*on_error)(const char*))
{
    if (on_error != NULL)
    {
        g_call_me_baybe.on_error = on_error;
    }
}

static void
try_this_as_callback(const char* msg)
{
    printf("HIHIH %s\n", msg);
    fflush(stdout);
}