#include <SDL.h>
#include <stdio.h>

#define UNREFERENCED_PARAMETER(x) (void)x

int main(int argc, char **argv) {
  UNREFERENCED_PARAMETER(argc);
  UNREFERENCED_PARAMETER(argv);

  printf("hello gamedev!\n");
  return 0;
}
