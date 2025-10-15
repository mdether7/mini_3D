#ifndef MY_FILE_READ_H
#define MY_FILE_READ_H

/**
 * @brief Reads entire file contents into memory (Stdlib)
 * @param filename Path to file to read
 * @return Dynamically allocated null-terminated string, or NULL on error
 * @note Caller must free() the returned pointer
 */
char* read_file(const char* filename);

/**
 * @brief Reads entire file contents into memory (Linux system calls)
 * @param filename Path to file to read
 * @return Dynamically allocated null-terminated string, or NULL on error
 * @note Caller must free() the returned pointer
 */
char* read_file_linux(const char* filename);

char* read_file_linux_debug(const char* filename);
char* read_file_linux_debug2(const char* filename);

// TODO: read_file_windows(const char* filename);


#endif /* MY_FILE_READ_H */