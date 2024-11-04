#!/bin/bash

BIN_NAME=techjam

mkdir -p web

emcc -Werror -Wall -Wextra -Oz -sUSE_SDL=2 -sMAX_WEBGL_VERSION=2 -sFULL_ES3 -I./include ./src/main.c ./src/**/*.c -o ./web/$BIN_NAME.html --preload-file ./assets
