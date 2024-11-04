#ifndef GL_H
#define GL_H

#ifdef __EMSCRIPTEN__
#include <GL/gl.h>
#include <GLES3/gl3.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#else
#include "glad/glad.h"
#endif

#endif
