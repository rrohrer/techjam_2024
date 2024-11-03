#!/bin/bash

BIN_NAME=techjam

mkdir -p web

emcc -Werror -Wall -Wextra -Oz -sUSE_SDL=2 -I./include ./src/main.c -o ./web/$BIN_NAME.html
