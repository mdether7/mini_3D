#ifndef M_SHADER_H
#define M_SHADER_H

#include <glad/glad.h>

typedef enum {
    PROGRAM_SLOT_0,
    PROGRAM_SLOT_1,
    PROGRAM_SLOT_2,
    PROGRAM_SLOT_3,
    PROGRAM_SLOT_4,
    PROGRAM_SLOT_5,
    PROGRAM_SLOT_6,
    PROGRAM_SLOT_7,
    PROGRAM_SLOT_8,
    PROGRAM_SLOT_9,
    MAX_SHADER_PROGRAMS
} ProgramSlot;

typedef enum {
    UNIFORM_MODEL,
    UNIFORM_VIEW,
    UNIFORM_PROJECTION,
    UNIFORM_TIME,
    UNIFORM_RESOLUTION,

    UNIFORM_POS_2D,
    UNIFORM_SIZE_2D,
    UNIFORM_COLOR,

    UNIFORM_TOTAL
} UniformLocation;

typedef struct s_shader_program {
    char*  name;
    GLuint handle;
    GLint  u_locations[UNIFORM_TOTAL];
} ShaderProgram;

extern ShaderProgram g_shader_programs[MAX_SHADER_PROGRAMS];

void shader_init_uniforms(ShaderProgram* program);
void shader_use_program(ProgramSlot slot); 

#endif