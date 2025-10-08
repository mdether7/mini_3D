#include "file_read.h"

#include <stdio.h>
#include <stdlib.h>

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