#include "render/gfx_api.h"

#include "gl.h"

#include <stdio.h>

struct Mesh mesh_new(void) {
  uint32_t vao = 0;
  glGenVertexArrays(1, (GLuint *)&vao);
  uint32_t vertex_buffer = 0;
  glGenBuffers(1, (GLuint *)&vertex_buffer);

  return (struct Mesh){.vertex_buffer = vertex_buffer, .vao = vao};
}

void mesh_fill(struct Mesh const *m, float const *data, size_t size) {
  glBindVertexArray(m->vao);
  glBindBuffer(GL_ARRAY_BUFFER, m->vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);
}

void mesh_bind(struct Mesh const *m) { glBindVertexArray(m->vao); }

void mesh_free(struct Mesh *m) {
  glDeleteBuffers(1, &m->vertex_buffer);
  glDeleteVertexArrays(1, &m->vao);
  *m = (struct Mesh){0};
}

// TODO: handle gracefully cleaning up after a failed shader, for now just count
// on exiting the program.
bool shader_new(struct Shader *shader, char const *vertex_src,
                char const *frag_src) {
  uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, (const GLchar *const *)&vertex_src, NULL);
  glCompileShader(vs);

  int32_t compile_status = 0;
  glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_status);
  if (!compile_status) {
    char error_msg[1024];
    glGetShaderInfoLog(vs, 1024, NULL, error_msg);
    printf("Vertex shader failed to compile:\n%s", error_msg);
    return false;
  }

  uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, (const GLchar *const *)&frag_src, NULL);
  glCompileShader(fs);

  glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_status);
  if (!compile_status) {
    char error_msg[1024];
    glGetShaderInfoLog(fs, 1024, NULL, error_msg);
    printf("Fragment shader failed to compile:\n%s", error_msg);
    return false;
  }

  uint32_t program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &compile_status);
  if (!compile_status) {
    char error_msg[1024];
    glGetProgramInfoLog(program, 1024, NULL, error_msg);
    printf("Shader program failed to link:\n%s", error_msg);
    return false;
  }

  *shader = (struct Shader){
      .program = program, .vertex_shader = vs, .fragment_shader = fs};
  return true;
}

void shader_free(struct Shader *s) {
  glDetachShader(s->program, s->vertex_shader);
  glDetachShader(s->program, s->fragment_shader);
  glDeleteShader(s->vertex_shader);
  glDeleteShader(s->fragment_shader);
  glDeleteProgram(s->program);
  *s = (struct Shader){0};
}

void shader_bind(struct Shader const *s) { glUseProgram(s->program); }

uint32_t shader_get_uniform(struct Shader const *s, char const *uniform_name) {
  return glGetUniformLocation(s->program, uniform_name);
}

void shader_set_matrix_uniform(uint32_t uniform_location,
                               struct Matrix4 const *m) {
  glUniformMatrix4fv(uniform_location, 1, GL_FALSE, (GLfloat const *)m);
}

void shader_set_vector_uniform(uint32_t uniform_location,
                               struct Vector4 const *v) {
  glUniform4fv(uniform_location, 1, (GLfloat const *)v);
}
