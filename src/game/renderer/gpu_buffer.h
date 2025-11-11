#ifndef DG_GPU_BUFFER_H
#define DG_GPU_BUFFER_H

#include <stddef.h>

#include <glad/glad.h>

// 1 MiB
#ifndef GPU_VERTEX_BUFFER_DEFAULT_SIZE
#define GPU_VERTEX_BUFFER_DEFAULT_SIZE (1024 * 1024)
#endif

// 0.5 MiB
#ifndef GPU_INDEX_BUFFER_DEFAULT_SIZE
#define GPU_INDEX_BUFFER_DEFAULT_SIZE (1024 * 512)
#endif

typedef struct {
    size_t offset_vbo;
    size_t offset_ebo;
    size_t max_capacity_vbo;
    size_t max_capacity_ebo;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
} DG3D_GpuBuffer;

int  dg3d_gpu_buffer_init(DG3D_GpuBuffer* buffer, size_t size_vbo, size_t size_ebo);
int  dg3d_gpu_buffer_send_vertex_data(DG3D_GpuBuffer* buffer, float* data, size_t size);
int  dg3d_gpu_buffer_send_index_data(DG3D_GpuBuffer* buffer, unsigned int* data, size_t size);
void dg3d_gpu_buffer_reset(DG3D_GpuBuffer* buffer);
void dg3d_gpu_buffer_destroy(DG3D_GpuBuffer* buffer);

#endif