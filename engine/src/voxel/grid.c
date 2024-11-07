#include "voxel/grid.h"

#include <stdlib.h>

struct Grid grid_new(uint32_t x, uint32_t y, uint32_t z,
                     struct Vector4 origin) {
  struct Grid result =
      (struct Grid){.origin = origin, .size_x = x, .size_y = y, .size_z = z};
  result.data = (char *)calloc(x * y * z, sizeof(char));
  result.color_palette[1] = Vector4_new_point(1.0, 0.0, 0.0);
  result.color_palette[2] = Vector4_new_point(0.0, 1.0, 0.0);
  return result;
}

void grid_free(struct Grid *grid) { free(grid->data); }

char grid_get(struct Grid const *grid, uint32_t x, uint32_t y, uint32_t z) {
  return grid->data[grid->size_x * grid->size_y * z + grid->size_x * y + x];
}

void grid_set(struct Grid *grid, uint32_t x, uint32_t y, uint32_t z,
              char value) {
  grid->data[grid->size_x * grid->size_y * z + grid->size_x * y + x] = value;
}
