#include "render/gfx_context.h"
#include "gl.h"
#include "platform/file.h"
#include "render/gfx_api.h"
#include "render/shader_files.h"

#include <SDL.h>
#include <stdlib.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

static const float g_cube_data[] = {
    -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f,
    0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
    0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, 0.5f,  -0.5f,
    0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,
    0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, 0.5f,
    -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, 0.5f,
    1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f,
    0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, 0.5f,
    0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f,
    0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  0.5f,
    0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

bool graphics_context_new(struct GraphicsContext *graphics) {
  SDL_Window *window = NULL;
  window = SDL_CreateWindow("TechJam 2024", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (NULL == window) {
    printf("Could not initialize SDL Window! SDL_Error: %s\n", SDL_GetError());
    return false;
  }
  uint32_t width = SCREEN_WIDTH;
  uint32_t height = SCREEN_HEIGHT;

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

  // float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f,
  // 0.0f};
  struct Mesh cube = mesh_new();
  mesh_fill(&cube, g_cube_data, sizeof(g_cube_data));

  struct File vs, fs;
  if (!file_read_all(&vs, BASIC_VS_PATH) ||
      !file_read_all(&fs, BASIC_FS_PATH)) {
    printf("Could not find shader files\n");
    return false;
  }
  struct Shader basic_lighting;
  if (!shader_new(&basic_lighting, vs.data, fs.data)) {
    printf("Could not compile shaders\n");
    return false;
  }

  file_free(&vs);
  file_free(&fs);

  shader_bind(&basic_lighting);

  uint32_t basic_lighting_view_proj =
      shader_get_uniform(&basic_lighting, "view_proj");
  uint32_t basic_lighting_model = shader_get_uniform(&basic_lighting, "model");
  uint32_t basic_lighting_color = shader_get_uniform(&basic_lighting, "color");
  uint32_t basic_lighting_ambient_dir =
      shader_get_uniform(&basic_lighting, "ambient_dir");
  uint32_t basic_lighting_ambient_color =
      shader_get_uniform(&basic_lighting, "ambient_color");
  uint32_t basic_lighting_light_pos =
      shader_get_uniform(&basic_lighting, "light_pos");
  uint32_t basic_lighting_light_color =
      shader_get_uniform(&basic_lighting, "light_color");
  uint32_t basic_lighting_camera_eye =
      shader_get_uniform(&basic_lighting, "camera_eye");
  uint32_t basic_lighting_fog_color =
      shader_get_uniform(&basic_lighting, "fog_color");
  uint32_t basic_lighting_fog_props =
      shader_get_uniform(&basic_lighting, "fog_props");

  SDL_GL_SwapWindow(window);

  *graphics = (struct GraphicsContext){
      .window = window,
      .width = width,
      .height = height,
      .basic_lighting = basic_lighting,
      .basic_lighting_color = basic_lighting_color,
      .basic_lighting_ambient_dir = basic_lighting_ambient_dir,
      .basic_lighting_ambient_color = basic_lighting_ambient_color,
      .basic_lighting_light_pos = basic_lighting_light_pos,
      .basic_lighting_light_color = basic_lighting_light_color,
      .basic_lighting_camera_eye = basic_lighting_camera_eye,
      .basic_lighting_fog_color = basic_lighting_fog_color,
      .basic_lighting_fog_props = basic_lighting_fog_props,
      .basic_lighting_view_proj = basic_lighting_view_proj,
      .basic_lighting_model = basic_lighting_model,
      .cube = cube};
  return true;
}
