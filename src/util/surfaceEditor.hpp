#pragma once

#include "surfaceCreator.hpp"

#include <SDL2/SDL.h>

namespace los {
	class SurfaceEditor {
		public:
			inline static SDL_Surface* crop(SDL_Surface* sf, signed short x, signed short y, signed short w, signed short h) {
				SDL_Rect src = { x, y, w, h };
				SDL_Surface *surface = SurfaceCreator::createEmptySurface(w, h);
				SDL_BlitSurface(sf, &src, surface, NULL);
				return surface;
			}
	};
};
