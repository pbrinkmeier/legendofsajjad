#pragma once

#include <SDL2/SDL.h>

namespace los {
	class Keyboard {
		public:
			inline static bool keyDown(int key) {
				const Uint8 *state = SDL_GetKeyboardState(NULL);
				return state[key];
			}
		public:
			inline static bool keyUp(int key) {
				return !keyDown(key);
			}
	};
};
