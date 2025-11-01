#include "shader.h"

#include <stdlib.h>
#include <assert.h>

#include "platform/file_read.h"
#include "platform/platform_tools.h"
#include "platform/platform_log.h"

// TODO: add shader compilation from char* directly.

static int shader_compile_error(GLuint shader);
static int shader_program_link_error(GLuint program);

void shader_program_delete(Shader* shader)
{
    if (shader && shader->id != 0) {
        glDeleteProgram(shader->id);
        shader->id = 0;
    }
}

void shader_program_bind(Shader* shader)
{
    glUseProgram(shader->id);
}

Shader shader_program_compile_from_path(const char* vert_path, const char* frag_path)
{  
    assert(vert_path && frag_path);
    RELEASE_ASSERT(vert_path && frag_path);

    char*         vertex_source   = NULL;
    char*         fragment_source = NULL;
    Shader        shader          = {0};

    vertex_source   = read_file(vert_path);
    fragment_source = read_file(frag_path);

    if (!vertex_source || !fragment_source) {
        free(vertex_source);
        free(fragment_source);
        return shader;
    }

    GLuint vertex_shader_id   = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader_id, 1, (const GLchar**)&vertex_source, NULL);
    glShaderSource(fragment_shader_id, 1, (const GLchar**)&fragment_source, NULL);

    free(vertex_source);
    free(fragment_source);
    vertex_source   = NULL;
    fragment_source = NULL;

    glCompileShader(vertex_shader_id);
    glCompileShader(fragment_shader_id);

    if (shader_compile_error(vertex_shader_id)   > 0 ||
        shader_compile_error(fragment_shader_id) > 0) {

        glDeleteShader(vertex_shader_id);  
        glDeleteShader(fragment_shader_id);

        return shader;
    }

    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    glDeleteShader(vertex_shader_id);  
    glDeleteShader(fragment_shader_id);

    if (shader_program_link_error(program_id) > 0) {

        glDeleteProgram(program_id);
        return shader;

    }

    shader.id = program_id;

    return shader;
}

Shader shader_program_tess_compile_from_path(const char* vert_path, 
    const char* tcs_path, const char* tes_path, const char* frag_path)
{  
    assert(vert_path && tcs_path && tes_path && frag_path);
    RELEASE_ASSERT(vert_path && tcs_path && tes_path && frag_path);

    char*         vertex_source       = NULL;
    char*         tess_control_source = NULL;
    char*         tess_eval_source    = NULL;
    char*         fragment_source     = NULL;
    Shader        shader              = {0};

    vertex_source       = read_file(vert_path);
    tess_control_source = read_file(tcs_path);
    tess_eval_source    = read_file(tes_path);
    fragment_source     = read_file(frag_path);

    if (!vertex_source || !fragment_source || !tess_control_source || !tess_eval_source) {
        free(vertex_source);
        free(tess_control_source);
        free(tess_eval_source);
        free(fragment_source);
        return shader;
    }

    GLuint vertex_shader_id   = glCreateShader(GL_VERTEX_SHADER);
    GLuint tess_control_id    = glCreateShader(GL_TESS_CONTROL_SHADER);
    GLuint tess_eval_id       = glCreateShader(GL_TESS_EVALUATION_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertex_shader_id, 1, (const GLchar**)&vertex_source, NULL);
    glShaderSource(tess_control_id, 1, (const GLchar**)&tess_control_source, NULL);
    glShaderSource(tess_eval_id, 1, (const GLchar**)&tess_eval_source, NULL);
    glShaderSource(fragment_shader_id, 1, (const GLchar**)&fragment_source, NULL);

    free(vertex_source);
    free(tess_control_source);
    free(tess_eval_source);
    free(fragment_source);
    vertex_source       = NULL;
    tess_control_source = NULL;
    tess_eval_source    = NULL;
    fragment_source     = NULL;

    glCompileShader(vertex_shader_id);
    glCompileShader(tess_control_id);
    glCompileShader(tess_eval_id);
    glCompileShader(fragment_shader_id);

    if (shader_compile_error(vertex_shader_id)   > 0 ||
        shader_compile_error(tess_control_id)    > 0 ||
        shader_compile_error(tess_eval_id)       > 0 ||
        shader_compile_error(fragment_shader_id) > 0) {

        glDeleteShader(vertex_shader_id);  
        glDeleteShader(tess_control_id);
        glDeleteShader(tess_eval_id);
        glDeleteShader(fragment_shader_id);

        return shader;
    }

    GLuint program_id = glCreateProgram();

    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, tess_control_id);
    glAttachShader(program_id, tess_eval_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    glDeleteShader(vertex_shader_id);  
    glDeleteShader(tess_control_id);
    glDeleteShader(tess_eval_id);
    glDeleteShader(fragment_shader_id);

    if (shader_program_link_error(program_id) > 0) {

        glDeleteProgram(program_id);
        return shader;

    }

    shader.id = program_id;

    return shader;
}

static int shader_compile_error(GLuint shader)
{
	GLint success;
	char info_log[1024] = {0};

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        platform_log_error("[SHADER COMPILE] Log: %s", info_log);
		return 1;
	}
	return 0;
}

static int shader_program_link_error(GLuint program)
{
    GLint success;
    char info_log[1024] = {0};

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, sizeof(info_log), NULL, info_log);
        platform_log_error("[SHADER LINK] Log: %s", info_log);
        return 1;
    }
    return 0;
}
