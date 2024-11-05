#ifndef GFX_CONTEXT_H
#define GFX_CONTEXT_H

#include <stdbool.h>

struct SDL_Window;

struct GraphicsContext {
  struct SDL_Window *window;
};

bool graphics_context_new(struct GraphicsContext *graphics);

#endif
