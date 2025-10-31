#ifndef PLATFORM_INPUT_H
#define PLATFORM_INPUT_H

// #define GLFW_KEY_A                  65
// #define GLFW_KEY_B                  66
// #define GLFW_KEY_C                  67
// #define GLFW_KEY_D                  68
// #define GLFW_KEY_E                  69
// #define GLFW_KEY_F                  70
// #define GLFW_KEY_G                  71
// #define GLFW_KEY_H                  72
// #define GLFW_KEY_I                  73
// #define GLFW_KEY_J                  74
// #define GLFW_KEY_K                  75
// #define GLFW_KEY_L                  76
// #define GLFW_KEY_M                  77
// #define GLFW_KEY_N                  78
// #define GLFW_KEY_O                  79
// #define GLFW_KEY_P                  80
// #define GLFW_KEY_Q                  81
// #define GLFW_KEY_R                  82
// #define GLFW_KEY_S                  83
// #define GLFW_KEY_T                  84
// #define GLFW_KEY_U                  85
// #define GLFW_KEY_V                  86
// #define GLFW_KEY_W                  87
// #define GLFW_KEY_X                  88
// #define GLFW_KEY_Y                  89
// #define GLFW_KEY_Z                  90

typedef enum {
    KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
    KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y ,KEY_Z, KEY_COUNT,   
} button_type;

typedef struct {
    unsigned char pressed;
    unsigned char held;
    unsigned char released;
} Button;
 
extern Button buttons[KEY_COUNT];

int platform_is_button_pressed(const button_type button);
void platform_internal_input_set_button(const button_type type, int action);

#endif