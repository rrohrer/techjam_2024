#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl.h"
#include "math/matrix4.h"
#include "math/vector4.h"
#include "render/gfx_context.h"
#include "voxel/grid.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define UNREFERENCED_PARAMETER(x) (void)x

struct Core {
  struct GraphicsContext graphics;
  struct Grid grid;
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
  //  struct Matrix4 m = Matrix4_orthographic(-width, width, -height, height);
  struct Matrix4 p =
      Matrix4_perspective(1.6f, (float)width / (float)height, 0.1f, 150.f);
  struct Matrix4 c = Matrix4_lookat(Vector4_new_point(10.f, 10.f, 10.f),
                                    Vector4_new_point(0.f, 0.f, 0.f),
                                    Vector4_new_vector(0.f, 1.f, 0.f));
  struct Matrix4 m = Matrix4_multiply(&p, &c);
  glClearColor(0.2f, 0.3f, 0.3f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_bind(&core.graphics.basic_lighting);
  mesh_bind(&core.graphics.cube);

  for (uint32_t z = 0; z < core.grid.size_z; ++z) {
    for (uint32_t y = 0; y < core.grid.size_y; ++y) {
      for (uint32_t x = 0; x < core.grid.size_x; ++x) {
        char voxel = grid_get(&core.grid, x, y, z);
        if (voxel != 0) {
          struct Matrix4 translation = Matrix4_translation(
              core.grid.origin.x + x, core.grid.origin.y + y,
              core.grid.origin.z + z);
          struct Matrix4 model = Matrix4_multiply(&m, &translation);
          shader_set_matrix_uniform(core.graphics.basic_lighting_transform,
                                    &model);
          // struct Vector4 player_color = {1.f, 0.5f, 0.2f, 0.f};
          shader_set_vector_uniform(core.graphics.basic_lighting_color,
                                    &core.grid.color_palette[(int)voxel]);
          glDrawArrays(GL_TRIANGLES, 0, CUBE_TRIGANGLE_COUNT);
        }
      }
    }
  }

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

  int grid_size = 20;
  core.grid =
      grid_new(grid_size, 10, grid_size,
               Vector4_new_point(-grid_size / 2.f, 0.0, -grid_size / 2.f));
  for (int i = 0; i < grid_size; ++i) {
    int counter = i % 2;
    for (int j = 0; j < grid_size; ++j, ++counter) {
      grid_set(&core.grid, i, 0, j, counter % 2 + 1);
    }
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
