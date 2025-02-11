#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL_events.h"
#include "core/debug.h"
#include "core/input.h"
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
  struct Debug debug;
  struct GraphicsContext graphics;
  struct Grid grid;
  struct Input input;
  struct World world;
  bool running;
};

static struct Core core;

static void mainloop(void) {
  // update input before processing new events
  input_update(&core.input);

  // process input from SDL
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      core.running = false;
      break;
    case SDL_KEYDOWN:
      input_register_keypress(&core.input,
                              input_translate_sdlkey(e.key.keysym.sym));
      break;
    case SDL_KEYUP:
      input_register_keyup(&core.input,
                           input_translate_sdlkey(e.key.keysym.sym));
    case SDL_MOUSEMOTION:
      input_register_mousemove(&core.input, e.motion.x, e.motion.y,
                               core.graphics.width, core.graphics.height);
    default:
      break;
    };
  }

  /*
  if (input_is_key_active(&core.input, KEYCODE_W)) {
    printf("W to win!\n");
  }

  if (core.input.mouse_moved) {
    printf("x: %f     y: %f\n ", core.input.mouse_pos.x,
           core.input.mouse_pos.y);
  }
  */

  // start rendering the frame
  glClearColor(core.world.fog_color.x, core.world.fog_color.y,
               core.world.fog_color.z, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glDepthMask(GL_TRUE);
  glEnable(GL_DEPTH_TEST);

  int width = core.graphics.width;
  int height = core.graphics.height;

  struct Matrix4 p =
      Matrix4_perspective(1.2f, (float)width / (float)height, 0.1f, 150.f);
  struct Matrix4 c =
      Matrix4_lookat(core.world.camera_eye, core.world.camera_target,
                     Vector4_new_vector(0.f, 1.f, 0.f));
  struct Matrix4 vp = Matrix4_multiply(&p, &c);

  // test picking
  if (core.input.is_mouse_valid) {
    struct Vector4 ndc_far =
        Vector4_new_point(core.input.mouse_pos.x, core.input.mouse_pos.y, 1.f);
    struct Vector4 ndc_near =
        Vector4_new_point(core.input.mouse_pos.x, core.input.mouse_pos.y, -1.f);
    struct Matrix4 ivp = Matrix4_invert(&vp);
    struct Vector4 world_far = Matrix4_transform(&ivp, ndc_far);
    world_far = Vector4_scale(world_far, 1.f / world_far.w);
    struct Vector4 world_near = Matrix4_transform(&ivp, ndc_near);
    world_near = Vector4_scale(world_near, 1.f / world_near.w);
    printf("world_near: %f %f %f %f\n", world_near.x, world_near.y,
           world_near.z, world_near.w);
    printf("world_far: %f %f %f %f\n", world_far.x, world_far.y, world_far.z,
           world_far.w);
    struct Vector4 world_dir = Vector4_subtract(world_far, world_near);
    printf("world_dir: %f %f %f\n", world_dir.x, world_dir.y, world_dir.z);
    Vector4_normalize(&world_dir);
    printf("|world_dir|: %f %f %f\n", world_dir.x, world_dir.y, world_dir.z);

    // struct Vector4 plane_o = Vector4_new_point(0.f, 0.f, 0.f);
    struct Vector4 plane_n = Vector4_new_vector(0.f, 1.f, 0.f);

    float wdotn = Vector4_dot(world_dir, plane_n);
    if (wdotn != 0.f) {
      float t = -Vector4_dot(world_near, plane_n) / wdotn;
      if (t > 0.f) {
        struct Vector4 s = Vector4_scale(world_dir, t);
        struct Vector4 hit = Vector4_add(world_near, s);
        printf("hit: %f %f %f %f\n", hit.x, hit.y, hit.z, t);
        struct Vector4 grid = Vector4_subtract(hit, core.grid.origin);
        uint32_t x = (uint32_t)grid.x;
        uint32_t y = (uint32_t)grid.y;
        uint32_t z = (uint32_t)grid.z;
        if (x < core.grid.size_x && y < core.grid.size_y &&
            z < core.grid.size_z) {
          grid_set(&core.grid, x, y, z, GRID_ORANGE);
        }
      }
    }
  }

  // render the scene
  shader_bind(&core.graphics.basic_lighting);
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
          shader_set_vector_uniform(core.graphics.basic_lighting_color,
                                    &core.grid.color_palette[(int)voxel]);
          glDrawArrays(GL_TRIANGLES, 0, CUBE_TRIGANGLE_COUNT);
        }
      }
    }
  }

  // update debug
  debug_add_aabb(&core.debug, Vector4_new_point(0.f, 0.f, 0.f),
                 Vector4_new_vector(2.5f, 2.5f, 2.5f), RED);
  debug_update(&core.debug, &vp);

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

  core.debug = debug_new();
  core.input = input_new();

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
