#include "gpu_buffer.h"

#include <assert.h>

#ifndef DG3D_GPU_DEFAULT_ALIGNMENT
#define DG3D_GPU_DEFAULT_ALIGNMENT (2 * sizeof(void *)) 
#endif

static int is_power_of_two(uintptr_t x) {
	return (x & (x-1)) == 0;
}

static size_t align_forward(size_t ptr, size_t align_to)
{
    // int modulo_result = ptr % align_to;
    // if (modulo_result == 0) return ptr;
    // return ptr + align_to - modulo_result; 
    assert(is_power_of_two(align_to));
    return (ptr + align_to - 1) & ~(align_to - 1);
}

int dg3d_gpu_buffer_init(DG3D_GpuBuffer* buffer, size_t size_vbo, size_t size_ebo)
{
    assert(buffer);
    assert(size_vbo > 0 && size_ebo > 0);

    // Create buffers.
    glGenVertexArrays(1, &buffer->VAO);

    glGenBuffers(1, &buffer->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO);
    glBufferData(GL_ARRAY_BUFFER, size_vbo, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &buffer->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_ebo, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Setup VAO 
    glBindVertexArray(buffer->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->EBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (sizeof(float) * 3), (void*)0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //! DONT UNBIND EBO HERE

    buffer->max_capacity_vbo = size_vbo;
    buffer->max_capacity_ebo = size_ebo;
    buffer->offset_vbo = 0;
    buffer->offset_ebo = 0;

    return 0;
}

int dg3d_gpu_buffer_send_vertex_data(DG3D_GpuBuffer* buffer, float* data, size_t size, size_t* out_offset)
{
    assert(buffer && data && out_offset);
    assert(size > 0);

    size_t aligned_offset = align_forward(buffer->offset_vbo, DG3D_GPU_DEFAULT_ALIGNMENT);

    if (size + aligned_offset <= buffer->max_capacity_vbo) {

        glBindBuffer(GL_ARRAY_BUFFER, buffer->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, aligned_offset, size, data);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        *out_offset = aligned_offset;
        buffer->offset_vbo = aligned_offset + size;
        return 0;
    } 

    return 1;
}

int dg3d_gpu_buffer_send_index_data(DG3D_GpuBuffer* buffer, unsigned int* data, size_t size, size_t* out_offset)
{
    assert(buffer && data && out_offset);
    assert(size > 0);

    size_t aligned_offset = align_forward(buffer->offset_ebo, DG3D_GPU_DEFAULT_ALIGNMENT);

    if (size + aligned_offset <= buffer->max_capacity_ebo) {
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, aligned_offset, size, data);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        *out_offset = aligned_offset;
        buffer->offset_ebo = aligned_offset + size;
        return 0;
    } 

    return 1;
}

void dg3d_gpu_buffer_reset(DG3D_GpuBuffer* buffer)
{
    assert(buffer);
    buffer->offset_vbo = 0;
    buffer->offset_ebo = 0;
}

void dg3d_gpu_buffer_destroy(DG3D_GpuBuffer* buffer)
{  
    if (buffer) { 
        glDeleteVertexArrays(1, &buffer->VAO);
        glDeleteBuffers(1, &buffer->VBO);
        glDeleteBuffers(1, &buffer->EBO);
        buffer->max_capacity_ebo = 0;
        buffer->max_capacity_vbo = 0;
        buffer->offset_ebo = 0;
        buffer->offset_vbo = 0;
    } 
}