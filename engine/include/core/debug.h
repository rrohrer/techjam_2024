#ifndef DEBUG_H
#define DEBUG_H

#include "math/vector4.h"
#include "render/gfx_api.h"

#include <stdint.h>
#include <stdlib.h>

struct Matrix4;

struct Line {
  struct Vector4 a;
  struct Vector4 color_a;
  struct Vector4 b;
  struct Vector4 color_b;
};

struct Debug {
  struct Line *lines;
  size_t lines_capacity;
  size_t lines_size;
  uint32_t lines_vao;
  uint32_t lines_vb;
  struct Shader lines_shader;
  uint32_t lines_shader_view_proj;
};

struct Debug debug_new(void);
void debug_free(struct Debug *debug);
void debug_add_line(struct Debug *debug, struct Vector4 a, struct Vector4 b,
                    struct Vector4 color);
void debug_update(struct Debug *debug, struct Matrix4 const *view_proj);

void debug_add_aabb(struct Debug *debug, struct Vector4 center,
                    struct Vector4 extents, struct Vector4 color);

#endif
