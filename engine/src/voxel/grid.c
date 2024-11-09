#include "voxel/grid.h"
#include "render/colors.h"
#include <stdlib.h>

struct Grid grid_new(uint32_t x, uint32_t y, uint32_t z,
                     struct Vector4 origin) {
  struct Grid result =
      (struct Grid){.origin = origin, .size_x = x, .size_y = y, .size_z = z};
  result.data = (char *)calloc(x * y * z, sizeof(char));
  result.color_palette[GRID_BEIGE] = BEIGE;
  result.color_palette[GRID_BEIGE_R] = BEIGE_R;
  result.color_palette[GRID_TAN] = TAN;
  result.color_palette[GRID_MAUVE] = MAUVE;
  result.color_palette[GRID_GREEN_BRIGHT] = GREEN_BRIGHT;
  result.color_palette[GRID_GREEN_LIGHT] = GREEN_LIGHT;
  result.color_palette[GRID_GREEN] = GREEN;
  result.color_palette[GRID_GREEN_DARK] = GREEN_DARK;
  result.color_palette[GRID_ORANGE] = ORANGE;
  result.color_palette[GRID_RED] = RED;

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
