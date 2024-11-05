#ifndef FILE_H
#define FILE_H
#include <stdbool.h>
#include <stdlib.h>

struct File {
  char *data;
  size_t length;
};

bool file_read_all(struct File *file, char const *path);
void file_free(struct File *file);
#endif
