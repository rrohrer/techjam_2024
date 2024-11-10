#include "core/input.h"

#include <SDL.h>

struct Input input_new(void) { return (struct Input){0}; }

void input_update(struct Input *input) {
  for (int i = 0; i < KEYCODE_COUNT; ++i) {
    if (input->key_state[i] == KEYSTATE_PRESSED ||
        input->key_state[i] == KEYSTATE_UP) {
      input->key_state[i] = (input->key_state[i] + 1) % KEYSTATE_COUNT;
    }
  }

  input->mouse_moved = false;
}

void input_register_keypress(struct Input *input, enum Keycode keycode) {
  input->key_state[keycode] = KEYSTATE_PRESSED;
}

void input_register_keyup(struct Input *input, enum Keycode keycode) {
  input->key_state[keycode] = KEYSTATE_UP;
}

void input_register_mousemove(struct Input *input, uint32_t x, uint32_t y,
                              uint32_t width, uint32_t height) {
  input->mouse_moved = true;
  input->is_mouse_valid = true;

  // translate this into GL style NDC
  input->mouse_pos.x = 2.f * ((x / (float)width) - 0.5f);
  input->mouse_pos.y = -2.f * ((y / (float)height) - 0.5f);
}

bool input_is_key_active(const struct Input *input, enum Keycode key) {
  return input->key_state[key] == KEYSTATE_DOWN ||
         input->key_state[key] == KEYSTATE_PRESSED;
}

enum Keycode input_translate_sdlkey(uint32_t key) {
  switch (key) {
  case SDLK_a:
    return KEYCODE_A;
  case SDLK_d:
    return KEYCODE_D;
  case SDLK_s:
    return KEYCODE_S;
  case SDLK_w:
    return KEYCODE_W;
  default:
    return KEYCODE_UNSUPPORTED;
  };
}
