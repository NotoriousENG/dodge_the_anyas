#include "sinput.h"

void doInput(KeyState *keyboard) {
  // avoid input lag by using keystates
  const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
  for (int i = 0; i < MAX_KEYBOARD_KEYS; i++) {
    if (currentKeyStates[i]) {
      if (keyboard[i] == NOT_PRESSED) {
        keyboard[i] = JUST_PRESSED;
      } else {
        keyboard[i] = PRESSED;
      }
    } else {
      keyboard[i] = NOT_PRESSED;
    }
  }
}