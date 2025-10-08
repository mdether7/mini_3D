#ifndef MY_FILE_READ_H
#define MY_FILE_READ_H

/**
 * @brief Reads entire file contents into memory
 * @param filename Path to file to read
 * @return Dynamically allocated null-terminated string, or NULL on error
 * @note Caller must free() the returned pointer
 */
char* read_file(const char* filename);


#endif /* MY_FILE_READ_H */