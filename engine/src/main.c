#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
