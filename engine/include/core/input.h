#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>

#include "math/vector4.h"

enum Keycode {
  KEYCODE_UNSUPPORTED,
  KEYCODE_A,
  KEYCODE_D,
  KEYCODE_S,
  KEYCODE_W,
  KEYCODE_COUNT
};
enum Keystate {
  KEYSTATE_NONE,
  KEYSTATE_PRESSED,
  KEYSTATE_DOWN,
  KEYSTATE_UP,
  KEYSTATE_COUNT
};

struct Input {
  enum Keystate key_state[KEYCODE_COUNT];
  // translate this into GL style NDC coordinates from -1 to 1
  struct Vector4 mouse_pos;
  bool mouse_moved;
  bool is_mouse_valid;
};

struct Input input_new(void);
// update needs to be called before input is polled for the frame.
// it forwards the state from the last frame.
void input_update(struct Input *input);
void input_register_keypress(struct Input *input, enum Keycode keycode);
void input_register_keyup(struct Input *input, enum Keycode keycode);
void input_register_mousemove(struct Input *input, uint32_t x, uint32_t y,
                              uint32_t width, uint32_t height);
// active key is PRESSED OR DOWN
bool input_is_key_active(struct Input const *input, enum Keycode key);
enum Keycode input_translate_sdlkey(uint32_t key);

#endif
