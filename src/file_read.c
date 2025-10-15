#include "file_read.h"

#include <stdio.h>
#include <stdlib.h>
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

char* read_file_linux_debug(const char* filename)
{
    printf("=== READ_FILE_LINUX DEBUG ===\n");
    printf("Attempting to read: %s\n", filename);
    
    char* contents;
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        fprintf(stderr, "Failed while opening a file: %s\n", filename);
        return NULL;
    }
    
    printf("File descriptor: %d\n", fd);
    
    struct stat st;
    int result = fstat(fd, &st);
    if (result == -1) {
        perror("fstat");
        fprintf(stderr, "Failed while statting a file: %s\n", filename);
        close(fd);
        return NULL;
    }
    
    off_t file_size = st.st_size;
    printf("File size: %ld bytes\n", file_size);
    printf("File mtime: %ld\n", st.st_mtime);
    
    contents = malloc(file_size + 1);
    if (contents == NULL) {
        fprintf(stderr, "Failed when allocating memory: %s\n", filename);
        close(fd);
        return NULL;
    }
    
    printf("Allocated buffer at: %p\n", (void*)contents);
    
    ssize_t bytes_read = read(fd, contents, file_size);
    if (bytes_read == -1) {
        fprintf(stderr, "Failed to read whole file: %s\n", filename);
        free(contents);
        close(fd);
        return NULL;
    }
    
    printf("Bytes read: %zd\n", bytes_read);
    contents[file_size] = '\0';
    
    printf("First 100 chars: %.100s\n", contents);
    printf("Last 50 chars: %s\n", contents + (file_size > 50 ? file_size - 50 : 0));
    
    close(fd);
    return contents;
}

char* read_file_linux_debug2(const char* filename)
{
    printf("=== READ_FILE_LINUX DEBUG ===\n");
    printf("Attempting to read: %s\n", filename);
    
    // Check what the file looks like BEFORE opening
    system("stat shaders/default.frag 2>/dev/null | head -1");
    system("head -c 100 shaders/default.frag 2>/dev/null");
    printf("\n");
    
    char* contents;
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return NULL;
    }
    
    printf("File descriptor: %d\n", fd);
    
    struct stat st;
    int result = fstat(fd, &st);
    if (result == -1) {
        perror("fstat");
        close(fd);
        return NULL;
    }
    
    off_t file_size = st.st_size;
    printf("File size: %ld bytes\n", file_size);
    printf("File mtime: %ld\n", st.st_mtime);
    
    contents = malloc(file_size + 1);
    if (contents == NULL) {
        close(fd);
        return NULL;
    }
    
    printf("Allocated buffer at: %p\n", (void*)contents);
    
    ssize_t bytes_read = read(fd, contents, file_size);
    if (bytes_read == -1) {
        perror("read");
        free(contents);
        close(fd);
        return NULL;
    }
    
    printf("Bytes read: %zd\n", bytes_read);
    contents[file_size] = '\0';
    
    printf("IMMEDIATELY after read(): %.100s\n", contents);
    
    // Check if memcmp shows they're the same
    char* verify = malloc(file_size + 1);
    lseek(fd, 0, SEEK_SET);
    read(fd, verify, file_size);
    verify[file_size] = '\0';
    
    printf("Second read for verification: %.100s\n", verify);
    printf("Buffers match: %s\n", memcmp(contents, verify, file_size) == 0 ? "YES" : "NO");
    
    free(verify);
    close(fd);
    return contents;
}