#ifndef GRID_H
#define GRID_H

#include "math/vector4.h"

#include <stdint.h>

#define GRID_MAX_COLORS 16

#define GRID_EMPTY 0
#define GRID_BEIGE 1
#define GRID_BEIGE_R 2
#define GRID_TAN 3
#define GRID_MAUVE 4
#define GRID_GREEN_BRIGHT 5
#define GRID_GREEN_LIGHT 6
#define GRID_GREEN 7
#define GRID_GREEN_DARK 8
#define GRID_ORANGE 9
#define GRID_RED 10

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
