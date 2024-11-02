#!/bin/bash
SDL_CFLAGS=$(sdl2-config --cflags)
SDL_LDFLAGS=$(sdl2-config --static-libs)
BIN_NAME=techjam

mkdir -p ./bin

cc -Wall -Wextra -Werror -std=c99 -O3 $SDL_CFLAGS -I./include src/main.c $SDL_LDFLAGS -o ./bin/$BIN_NAME-rel
cc -Wall -Wextra -Werror -std=c99 $SDL_CFLAGS -I./include src/main.c $SDL_LDFLAGS -g -fsanitize=address -o ./bin/$BIN_NAME-asan
