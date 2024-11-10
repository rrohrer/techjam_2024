#ifndef SHADER_FILES_H
#define SHADER_FILES_H

#ifdef __EMSCRIPTEN__

#define BASIC_VS_PATH "assets/shaders/basic.webgl.vert"
#define BASIC_FS_PATH "assets/shaders/basic.webgl.frag"
#define LINE_VS_PATH "assets/shaders/line.webgl.vert"
#define LINE_FS_PATH "assets/shaders/line.webgl.frag"

#else

#define BASIC_VS_PATH "assets/shaders/basic.gl.vert"
#define BASIC_FS_PATH "assets/shaders/basic.gl.frag"
#define LINE_VS_PATH "assets/shaders/line.gl.vert"
#define LINE_FS_PATH "assets/shaders/line.gl.frag"

#endif

#endif
