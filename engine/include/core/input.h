#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>

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
};

struct Input input_new(void);
// update needs to be called before input is polled for the frame.
// it forwards the state from the last frame.
void input_update(struct Input *input);
void input_register_keypress(struct Input *input, enum Keycode keycode);
void input_register_keyup(struct Input *input, enum Keycode keycode);
// active key is PRESSED OR DOWN
bool input_is_key_active(struct Input const *input, enum Keycode key);
enum Keycode input_translate_sdlkey(uint32_t key);

#endif
