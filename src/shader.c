#include "shader.h"

#include <glad/glad.h>

ShaderProgram g_shader_programs[MAX_SHADER_PROGRAMS];

void
shader_init_unifroms(ShaderProgram* program)
{
    program->u_locations[UNIFORM_MODEL]      = glGetUniformLocation(program->handle, "u_model");
    program->u_locations[UNIFORM_VIEW]       = glGetUniformLocation(program->handle, "u_view");
    program->u_locations[UNIFORM_PROJECTION] = glGetUniformLocation(program->handle, "u_projection");
    program->u_locations[UNIFORM_TIME]       = glGetUniformLocation(program->handle, "u_time");
    program->u_locations[UNIFORM_RESOLUTION] = glGetUniformLocation(program->handle, "u_resolution");
}

void 
shader_use_program(ProgramType slot)
{
    glUseProgram(g_shader_programs[slot].handle);
}

