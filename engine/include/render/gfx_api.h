#ifndef GFX_API_H
#define GFX_API_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

struct Matrix4;
struct Vector4;

struct Shader {
  uint32_t program;
  uint32_t vertex_shader;
  uint32_t fragment_shader;
};

struct Mesh {
  uint32_t vertex_buffer;
  uint32_t vao;
};

struct Mesh mesh_new(void);

void mesh_fill(struct Mesh const *m, float const *data, size_t size);

void mesh_bind(struct Mesh const *m);

void mesh_free(struct Mesh *m);

bool shader_new(struct Shader *shader, char const *vertex_src,
                char const *frag_src);

void shader_free(struct Shader *s);

void shader_bind(struct Shader const *s);

uint32_t shader_get_uniform(struct Shader const *s, char const *uniform_name);

void shader_set_matrix_uniform(uint32_t uniform_location,
                               struct Matrix4 const *m);

void shader_set_vector_uniform(uint32_t uniform_location,
                               struct Vector4 const *v);

#endif
