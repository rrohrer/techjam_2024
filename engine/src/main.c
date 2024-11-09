#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl.h"
#include "math/matrix4.h"
#include "math/vector4.h"
#include "render/colors.h"
#include "render/gfx_api.h"
#include "render/gfx_context.h"
#include "voxel/grid.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define UNREFERENCED_PARAMETER(x) (void)x

struct World {
  struct Vector4 ambient_dir;
  struct Vector4 ambient_color;
  struct Vector4 point_light_pos;
  struct Vector4 point_light_color;
  struct Vector4 fog_color;
  float fog_start;
  float fog_end;
  struct Vector4 camera_eye;
  struct Vector4 camera_target;
};

struct Core {
  struct GraphicsContext graphics;
  struct Grid grid;
  struct World world;
  bool running;
};

static struct Core core;

static void mainloop(void) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT)
      core.running = false;
  }

  int width = core.graphics.width;
  int height = core.graphics.height;
  //  struct Matrix4 m = Matrix4_orthographic(-width, width, -height, height);
  struct Matrix4 p =
      Matrix4_perspective(1.6f, (float)width / (float)height, 0.1f, 150.f);
  struct Matrix4 c =
      Matrix4_lookat(core.world.camera_eye, core.world.camera_target,
                     Vector4_new_vector(0.f, 1.f, 0.f));
  struct Matrix4 vp = Matrix4_multiply(&p, &c);
  shader_set_matrix_uniform(core.graphics.basic_lighting_view_proj, &vp);
  shader_set_vector_uniform(core.graphics.basic_lighting_ambient_color,
                            &core.world.ambient_color);
  shader_set_vector_uniform(core.graphics.basic_lighting_ambient_dir,
                            &core.world.ambient_dir);
  shader_set_vector_uniform(core.graphics.basic_lighting_light_pos,
                            &core.world.point_light_pos);
  shader_set_vector_uniform(core.graphics.basic_lighting_light_color,
                            &core.world.point_light_color);
  shader_set_vector_uniform(core.graphics.basic_lighting_camera_eye,
                            &core.world.camera_eye);
  shader_set_vector_uniform(core.graphics.basic_lighting_fog_color,
                            &core.world.fog_color);
  struct Vector4 fog_props =
      Vector4_new_vector(core.world.fog_start, core.world.fog_end, 0.f);
  shader_set_vector_uniform(core.graphics.basic_lighting_fog_props, &fog_props);

  glClearColor(core.world.fog_color.x, core.world.fog_color.y,
               core.world.fog_color.z, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shader_bind(&core.graphics.basic_lighting);
  mesh_bind(&core.graphics.cube);

  for (uint32_t z = 0; z < core.grid.size_z; ++z) {
    for (uint32_t y = 0; y < core.grid.size_y; ++y) {
      for (uint32_t x = 0; x < core.grid.size_x; ++x) {
        char voxel = grid_get(&core.grid, x, y, z);
        if (voxel != 0) {
          struct Matrix4 model = Matrix4_translation(core.grid.origin.x + x,
                                                     core.grid.origin.y + y,
                                                     core.grid.origin.z + z);
          shader_set_matrix_uniform(core.graphics.basic_lighting_model, &model);
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

  core.world.ambient_dir = Vector4_new_vector(-0.2f, -0.8f, 0.2f);
  core.world.ambient_color = Vector4_new_vector(0.2f, 0.2f, 0.2f);
  core.world.point_light_pos = Vector4_new_point(0.f, 4.f, 0.f);
  core.world.point_light_color = Vector4_new_vector(1.f, 1.f, 1.f);
  core.world.fog_color = GREEN_DARK;
  core.world.fog_start = 10.f;
  core.world.fog_end = 30.f;
  core.world.camera_eye = Vector4_new_point(10.f, 10.f, 10.f);
  core.world.camera_target = Vector4_new_point(0.f, 0.f, 0.f);

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
