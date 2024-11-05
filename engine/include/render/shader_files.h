#ifndef SHADER_FILES_H
#define SHADER_FILES_H

#ifdef __EMSCRIPTEN__

#define BASIC_VS_PATH "assets/shaders/basic.webgl.vs"
#define BASIC_FS_PATH "assets/shaders/basic.webgl.fs"

#else

#define BASIC_VS_PATH "assets/shaders/basic.gl.vs"
#define BASIC_FS_PATH "assets/shaders/basic.gl.fs"

#endif

#endif
