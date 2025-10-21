#include "shader.h"

#include <glad/glad.h>

#include "mini_utils.h"

ShaderProgram g_shader_programs[MAX_SHADER_PROGRAMS];

static const char* shader_uniform_to_string(UniformLocation loc);

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

#ifdef DEBUG
    util_print(TERMINAL_CYAN "[Shader program name: %s] (handle=%u)\n", program->name, program->handle);

    for (int loc = 0; loc < UNIFORM_TOTAL; loc++) {
        const char* info;
        info = (program->u_locations[loc] == -1) ? "NOT SET" : "SET";
        util_print("%s: %s\n", shader_uniform_to_string(loc), info);
    }

    util_print_n_flush(TERMINAL_RESET);
#endif
}

void 
shader_use_program(ProgramSlot slot)
{
    glUseProgram(g_shader_programs[slot].handle);
}

void
shader_delete_program(ProgramSlot slot)
{
    glDeleteProgram(g_shader_programs[slot].handle);
}

static const char*
shader_uniform_to_string(UniformLocation loc)
{
    switch (loc)
    {
        case UNIFORM_MODEL:       return "u_model";
        case UNIFORM_VIEW:        return "u_view";
        case UNIFORM_PROJECTION:  return "u_projection";
        case UNIFORM_TIME:        return "u_time";
        case UNIFORM_RESOLUTION:  return "u_resolution";

        case UNIFORM_POS_2D:      return "u_pos";
        case UNIFORM_SIZE_2D:     return "u_size";
        case UNIFORM_COLOR:       return "u_color";

        case UNIFORM_TOTAL:       break; // fallthrough to default
    }

    return "<invalid_uniform>";
}


