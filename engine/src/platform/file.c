#include "platform/file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool file_read_all(struct File *file, char const *path) {
  bool result = false;

  FILE *fp = fopen(path, "rb");
  if (fp == NULL) {
    printf("Could not find file: %s\n", path);

    char buffer[1024] = "../";
    strcat(buffer, path);
    fp = fopen(buffer, "rb");
    if (fp == NULL) {
      printf("Could not find file:%s\n", buffer);
      goto exit;
    }
  }

  fseek(fp, 0L, SEEK_END);
  size_t length = ftell(fp);
  rewind(fp);

  char *data = (char *)malloc(length + 1);
  if (data == NULL) {
    printf("Could not allocate memory for file: %s\n", path);
    goto exit;
  }

  fseek(fp, 0L, SEEK_SET);

  size_t read = fread(data, sizeof(char), length, fp);
  if (read != length) {
    printf("Failed to read all of file: %s\nread: %d len: %d\n", path,
           (int)read, (int)length);
    goto exit;
  }

  file->data = data;
  file->length = length;
  file->data[length] = 0;

  result = true;

exit:
  fclose(fp);
  if (!result) {
    free(data);
  }

  return result;
}

void file_free(struct File *file) { free(file->data); }
