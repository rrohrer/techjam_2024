#include "core/debug.h"

#include "platform/file.h"
#include "render/gfx_api.h"
#include "render/shader_files.h"

#include "gl.h"

#include <stdio.h>

struct Debug debug_new(void) {
  size_t capacity = 128;
  struct Line *lines = (struct Line *)calloc(capacity, sizeof(struct Line));
  if (lines == NULL) {
    printf("Failed to allocate buffer for lines\n");
  }

  uint32_t vao = 0;
  uint32_t vb = 0;
  glGenVertexArrays(1, (GLuint *)&vao);
  glGenBuffers(1, (GLuint *)&vb);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(struct Vector4),
                        (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(struct Vector4),
                        (void *)(sizeof(struct Vector4)));
  glEnableVertexAttribArray(1);

  struct File vs, fs;
  if (!file_read_all(&vs, LINE_VS_PATH) || !file_read_all(&fs, LINE_FS_PATH)) {
    printf("Could not find line drawing shader files\n");
    exit(1);
  }
  struct Shader lines_shader;
  if (!shader_new(&lines_shader, vs.data, fs.data)) {
    printf("Could not compile line drawing shaders\n");
    exit(1);
  }

  shader_bind(&lines_shader);
  uint32_t lines_shader_view_proj =
      shader_get_uniform(&lines_shader, "view_proj");

  return (struct Debug){.lines = lines,
                        .lines_capacity = capacity,
                        .lines_size = 0,
                        .lines_vao = vao,
                        .lines_vb = vb,
                        .lines_shader = lines_shader,
                        .lines_shader_view_proj = lines_shader_view_proj};
}

void debug_free(struct Debug *debug) {
  free(debug->lines);
  glDeleteBuffers(1, &debug->lines_vb);
  glDeleteVertexArrays(1, &debug->lines_vao);
  shader_free(&debug->lines_shader);
  *debug = (struct Debug){0};
}

void debug_add_line(struct Debug *debug, struct Vector4 a, struct Vector4 b,
                    struct Vector4 color) {
  if (debug->lines_size >= debug->lines_capacity) {
    size_t new_capacity = debug->lines_capacity * 2;
    debug->lines = (struct Line *)realloc(debug->lines,
                                          new_capacity * sizeof(struct Line));
    if (debug->lines == NULL) {
      printf("Failed to resize Debug lines\n");
      return;
    }
    debug->lines_capacity = new_capacity;
  }

  debug->lines[debug->lines_size++] =
      (struct Line){.a = a, .b = b, .color_a = color, .color_b = color};
}

void debug_update(struct Debug *debug, struct Matrix4 const *view_proj) {
  if (debug->lines_size == 0)
    return;

  glBindVertexArray(debug->lines_vao);
  glBufferData(GL_ARRAY_BUFFER, debug->lines_size * sizeof(struct Line),
               debug->lines, GL_STATIC_DRAW);
  shader_bind(&debug->lines_shader);
  shader_set_matrix_uniform(debug->lines_shader_view_proj, view_proj);

  glDrawArrays(GL_LINES, 0, debug->lines_size * 2);

  debug->lines_size = 0;
}

void debug_add_aabb(struct Debug *debug, struct Vector4 center,
                    struct Vector4 extents, struct Vector4 color) {
  struct Vector4 a = Vector4_new_point(
      center.x + extents.x, center.y + extents.y, center.z + extents.z);
  struct Vector4 b = Vector4_new_point(
      center.x + extents.x, center.y + extents.y, center.z - extents.z);
  struct Vector4 c = Vector4_new_point(
      center.x + extents.x, center.y - extents.y, center.z + extents.z);
  struct Vector4 d = Vector4_new_point(
      center.x + extents.x, center.y - extents.y, center.z - extents.z);
  struct Vector4 e = Vector4_new_point(
      center.x - extents.x, center.y + extents.y, center.z + extents.z);
  struct Vector4 f = Vector4_new_point(
      center.x - extents.x, center.y + extents.y, center.z - extents.z);
  struct Vector4 g = Vector4_new_point(
      center.x - extents.x, center.y - extents.y, center.z + extents.z);
  struct Vector4 h = Vector4_new_point(
      center.x - extents.x, center.y - extents.y, center.z - extents.z);

  debug_add_line(debug, a, b, color);
  debug_add_line(debug, a, c, color);
  debug_add_line(debug, c, d, color);
  debug_add_line(debug, d, b, color);
  debug_add_line(debug, a, e, color);
  debug_add_line(debug, f, b, color);
  debug_add_line(debug, h, d, color);
  debug_add_line(debug, e, f, color);
  debug_add_line(debug, e, g, color);
  debug_add_line(debug, g, h, color);
  debug_add_line(debug, h, f, color);
  debug_add_line(debug, g, c, color);
}
