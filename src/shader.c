#include "shader.h"

#include <glad/glad.h>

ShaderProgram g_shader_programs[MAX_SHADER_PROGRAMS];

static void shader_print_debug_init_information(ShaderProgram* program);

void
shader_init_uniforms(ShaderProgram* program)
{
    program->u_locations[UNIFORM_MODEL]      = glGetUniformLocation(program->handle, "u_model");
    program->u_locations[UNIFORM_VIEW]       = glGetUniformLocation(program->handle, "u_view");
    program->u_locations[UNIFORM_PROJECTION] = glGetUniformLocation(program->handle, "u_projection");
    program->u_locations[UNIFORM_TIME]       = glGetUniformLocation(program->handle, "u_time");
    program->u_locations[UNIFORM_RESOLUTION] = glGetUniformLocation(program->handle, "u_resolution");
    program->u_locations[UNIFORM_POS_2D]     = glGetUniformLocation(program->handle, "u_pos");
    program->u_locations[UNIFORM_SIZE_2D]    = glGetUniformLocation(program->handle, "u_size");
    program->u_locations[UNIFORM_COLOR]      = glGetUniformLocation(program->handle, "u_color");
}

void 
shader_use_program(ProgramSlot slot)
{
    glUseProgram(g_shader_programs[slot].handle);
}

static void
shader_print_debug_init_information(ShaderProgram* program)
{
    for (int i = 0; i < UNIFORM_TOTAL; i++) {
        
    }
}

