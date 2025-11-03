#ifndef MY_FILE_READ_H
#define MY_FILE_READ_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Reads entire file contents into memory (Stdlib)
 * @param filename Path to file to read
 * @return Dynamically allocated null-terminated string, or NULL on error
 * @note Caller must free() the returned pointer
 */
char* read_file(const char* filename);

/**
 * @brief Reads entire file contents into memory as raw bytes.
 * @param filename Path to file to read
 * @param file_size Raw size in bytes of returned data
 * @return Dynamically allocated binary data, or NULL on error
 * @note Caller must free() the returned pointer
 */
unsigned char* read_file_binary(const char* filename, size_t* file_size);

/**
 * @brief Write piece of data to a file as raw binary.
 * @param data pointer to data.
 * @param size size of data to be read.
 * @param filename Path to file.
 * @return True on success, else false.
 */
bool write_data_to_file_binary(void* data, size_t size, const char* filename);

/**
 * @#include "platform/file_read.h"
#include "platform/platform_log.h"brief Read file into some data buffer.
 * @param data pointer to data.
 * @param filename Path to file.
 * @return True on success, else false.
 */
bool read_data_from_file_binary(void* data, const char* filename);

/**
 * @brief Reads entire file contents into memory (Linux system calls)
 * @param filename Path to file to read
 * @return Dynamically allocated null-terminated string, or NULL on error
 * @note Caller must free() the returned pointer
 */
char* read_file_linux(const char* filename);
// TODO: read_file_windows(const char* filename);

#endif /* MY_FILE_READ_H */