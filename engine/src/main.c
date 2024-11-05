#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl.h"
#include "math/matrix4.h"
#include "render/gfx_context.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define UNREFERENCED_PARAMETER(x) (void)x

struct Core {
  struct GraphicsContext graphics;
  bool running;
};

static struct Core core;

static void mainloop(void) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT)
      core.running = false;
  }

  int width = core.graphics.width / 40;
  int height = core.graphics.height / 40;
  struct Matrix4 m = Matrix4_orthographic(-width, width, -height, height);

  glClearColor(0.2f, 0.3f, 0.3f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_bind(&core.graphics.basic_lighting);
  shader_set_matrix_uniform(core.graphics.basic_lighting_transform, &m);
  struct Vector4 player_color = {1.f, 0.5f, 0.2f, 0.f};
  shader_set_vector_uniform(core.graphics.basic_lighting_color, &player_color);
  mesh_bind(&core.graphics.cube);
  glDrawArrays(GL_TRIANGLES, 0, CUBE_TRIGANGLE_COUNT);

  SDL_GL_SwapWindow(core.graphics.window);
}

int main(int argc, char **argv) {
  UNREFERENCED_PARAMETER(argc);
  UNREFERENCED_PARAMETER(argv);
  int exit_code = EXIT_SUCCESS;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (!graphics_context_new(&core.graphics)) {
    printf("Failed to initialize graphics\n");
    exit_code = EXIT_FAILURE;
    goto cleanup;
  }

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(mainloop, 0, 1);
#else
  core.running = true;
  while (core.running) {
    mainloop();
  }
#endif

cleanup:
  SDL_DestroyWindow(core.graphics.window);
  SDL_Quit();
  return exit_code;
}
