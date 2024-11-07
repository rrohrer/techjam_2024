#ifndef GRID_H
#define GRID_H

#include "math/vector4.h"

#include <stdint.h>

#define GRID_MAX_COLORS 16

struct Grid {
  struct Vector4 origin;
  uint32_t size_x;
  uint32_t size_y;
  uint32_t size_z;
  char *data;
  struct Vector4 color_palette[GRID_MAX_COLORS];
};

struct Grid grid_new(uint32_t x, uint32_t y, uint32_t z, struct Vector4 origin);
void grid_free(struct Grid *grid);
char grid_get(struct Grid const *grid, uint32_t x, uint32_t y, uint32_t z);
void grid_set(struct Grid *grid, uint32_t x, uint32_t y, uint32_t z,
              char value);

#endif
