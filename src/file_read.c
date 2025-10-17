#include "file_read.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <string.h>

char* read_file(const char* filename)
{
    char* contents;

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("fopen");
        fprintf(stderr, "Failed while opening a file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    long file_size = ftell(file);
    if (file_size == 0) {
        fclose(file);
        return NULL;
    }
    fseek(file, 0L, SEEK_SET);
  
    contents = malloc(file_size + 1);
    if (contents == NULL) {
        fprintf(stderr, "Failed when allocating memory: %s\n", filename);
        fclose(file);
        return NULL;
    }
    
    size_t bytes_read = fread(contents, 1, file_size, file);
    if ((long)bytes_read != file_size) {
        fprintf(stderr, "Failed to read whole file: %s\n", filename);
        free(contents);
        fclose(file);
        return NULL;
    }

    contents[file_size] = '\0';

    fclose(file);
    return contents;
}

unsigned char* read_file_binary(const char* filename, size_t* file_size)
{
    unsigned char* data;

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("fopen");
        fprintf(stderr, "Failed while opening a file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    long internal_file_size = ftell(file);
    if (internal_file_size == 0) {
        fclose(file);
        return NULL;
    }
    fseek(file, 0L, SEEK_SET);

    data = malloc((size_t)internal_file_size);
    if (data == NULL) {
        fprintf(stderr, "Failed when allocating memory: %s\n", filename);
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(data, 1, internal_file_size, file);
    if (bytes_read != (size_t)internal_file_size) {
        fprintf(stderr, "Failed to read whole file: %s\n", filename);
        free(data);
        fclose(file);
        return NULL;
    }

    fclose(file);

    *file_size = internal_file_size;
    return data;
}

bool write_data_to_file_binary(void* data, size_t size, const char* filename)
{
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("fopen");
        fprintf(stderr, "Failed while opening a file: %s\n", filename);
        return false;
    }

    size_t bytes_wrote = fwrite(data, size, 1, file);
    




    return true;
}

char* read_file_linux(const char* filename)
{
    char* contents;

    int fd = open(filename, O_RDONLY | O_SYNC);
    if (fd == -1) {
        perror("open");
        fprintf(stderr, "Failed while opening a file: %s\n", filename);
        return NULL;
    }

    // One way to do it on linux
    // off_t file_size = lseek(fd, 0, SEEK_END);
    // lseek(fd, 0, SEEK_SET);

    struct stat st;
    int result = fstat(fd, &st);
    if (result == -1) {
        perror("fstat");
        fprintf(stderr, "Failed fstat: %s\n", filename);
        close(fd);
        return NULL;
    }
    off_t file_size = st.st_size;

    contents = malloc(file_size + 1);
    if (contents == NULL) {
        fprintf(stderr, "Failed when allocating memory: %s\n", filename);
        close(fd);
        return NULL;
    }

    ssize_t total_read = 0;
    while (total_read < file_size) {
        ssize_t n = read(fd, contents + total_read, file_size - total_read);
        if (n == -1) {
            perror("read");
            fprintf(stderr, "Failed while reading file: %s\n", filename);
            free(contents);
            close(fd);
            return NULL;
        }
        
        if (n == 0)
            break;

        total_read += n;
    }

    contents[file_size] = '\0';

    close(fd);
    return contents;
}