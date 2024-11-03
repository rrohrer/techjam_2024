#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define UNREFERENCED_PARAMETER(x) (void)x
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

static bool quit = false;

static void mainloop(void) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT)
      quit = true;
  }
}
int main(int argc, char **argv) {
  UNREFERENCED_PARAMETER(argc);
  UNREFERENCED_PARAMETER(argv);
  SDL_Window *window = NULL;
  SDL_Surface *screen_surface = NULL;
  int exit_code = EXIT_SUCCESS;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Could not initialize SDL! SDL_Error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  window = SDL_CreateWindow("Ferguson and the Half Robot",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (NULL == window) {
    printf("Could not initialize SDL Window! SDL_Error: %s\n", SDL_GetError());
    exit_code = EXIT_FAILURE;
    goto cleanup;
  }

  screen_surface = SDL_GetWindowSurface(window);
  SDL_FillRect(screen_surface, NULL,
               SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));
  SDL_UpdateWindowSurface(window);

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(mainloop, 0, 1);
#else
  while (!quit) {
    mainloop();
  }
#endif

cleanup:
  SDL_DestroyWindow(window);
  SDL_Quit();
  return exit_code;
}
