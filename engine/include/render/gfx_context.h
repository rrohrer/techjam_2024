#ifndef GFX_CONTEXT_H
#define GFX_CONTEXT_H

#include <render/gfx_api.h>
#include <stdbool.h>

struct SDL_Window;

struct GraphicsContext {
  struct SDL_Window *window;
  struct Mesh cube;
  struct Shader basic_lighting;
  uint32_t basic_lighting_transform;
  uint32_t basic_lighting_color;
  uint32_t width;
  uint32_t height;
};

bool graphics_context_new(struct GraphicsContext *graphics);

#define CUBE_TRIGANGLE_COUNT 12 * 3

#endif
