#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl.h"
#include "math/matrix4.h"
#include "math/vector4.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define UNREFERENCED_PARAMETER(x) (void)x

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#ifdef __EMSCRIPTEN__

#define BASIC_VS_PATH "assets/shaders/basic.webgl.vs"
#define BASIC_FS_PATH "assets/shaders/basic.webgl.fs"

#else

#define BASIC_VS_PATH "assets/shaders/basic.gl.vs"
#define BASIC_FS_PATH "assets/shaders/basic.gl.fs"

#endif

static bool quit = false;

static void mainloop(void) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT)
      quit = true;
  }
}

struct File {
  char *data;
  size_t length;
};

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

struct GraphicsContext {
  SDL_Window *window;
};

struct Shader {
  uint32_t program;
  uint32_t vertex_shader;
  uint32_t fragment_shader;
};

struct Mesh {
  uint32_t vertex_buffer;
  uint32_t vao;
};

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

bool graphics_context_new(struct GraphicsContext *graphics) {
  SDL_Window *window = NULL;
  window = SDL_CreateWindow("TechJam 2024", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (NULL == window) {
    printf("Could not initialize SDL Window! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  // Request an OpenGL 3.3 context (should be core)
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  // Also request a depth buffer
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

#ifdef __EMSCRIPTEN__
  EmscriptenWebGLContextAttributes attrs;
  emscripten_webgl_init_context_attributes(
      &attrs); // you MUST init the attributes before creating the context
  attrs.antialias = true;
  attrs.majorVersion = 2;
  attrs.minorVersion = 0;
  attrs.alpha = true;
  attrs.powerPreference = EM_WEBGL_POWER_PREFERENCE_DEFAULT;

  // The following lines must be done in exact order, or it will break!
  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE webgl_context =
      emscripten_webgl_create_context("#canvas", &attrs);
  emscripten_webgl_make_context_current(webgl_context);
#endif

  SDL_GL_CreateContext(window);

#ifndef __EMSCRIPTEN__
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    printf("Failed to initialize opengl...:%s\n", SDL_GetError());
    return false;
  }
#endif

  glClearColor(0.2f, 0.3f, 0.3f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
  struct Mesh triangle = mesh_new();
  mesh_fill(&triangle, vertices, sizeof(vertices));

  struct File vs, fs;
  if (!file_read_all(&vs, BASIC_VS_PATH) ||
      !file_read_all(&fs, BASIC_FS_PATH)) {
    printf("Could not find shader files\n");
    return false;
  }
  struct Shader basic;
  if (!shader_new(&basic, vs.data, fs.data)) {
    printf("Could not compile shaders\n");
    return false;
  }

  int width = SCREEN_WIDTH / 40;
  int height = SCREEN_HEIGHT / 40;
  struct Matrix4 m = Matrix4_orthographic(-width, width, -height, height);

  shader_bind(&basic);

  uint32_t m_loc = shader_get_uniform(&basic, "transform");
  uint32_t color_loc = shader_get_uniform(&basic, "color");

  shader_set_matrix_uniform(m_loc, &m);
  struct Vector4 player_color = {1.f, 0.5f, 0.2f, 0.f};
  shader_set_vector_uniform(color_loc, &player_color);
  mesh_bind(&triangle);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  SDL_GL_SwapWindow(window);

  *graphics = (struct GraphicsContext){.window = window};
  return true;
}

int main(int argc, char **argv) {
  UNREFERENCED_PARAMETER(argc);
  UNREFERENCED_PARAMETER(argv);
  int exit_code = EXIT_SUCCESS;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  struct GraphicsContext graphics_context = {0};
  if (!graphics_context_new(&graphics_context)) {
    printf("Failed to initialize graphics\n");
    exit_code = EXIT_FAILURE;
    goto cleanup;
  }

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(mainloop, 0, 1);
#else
  while (!quit) {
    mainloop();
  }
#endif

cleanup:
  SDL_DestroyWindow(graphics_context.window);
  SDL_Quit();
  return exit_code;
}
