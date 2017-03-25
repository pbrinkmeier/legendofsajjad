#pragma once

#include <SDL2/SDL.h>

namespace los {
	class SurfaceCreator {
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			#define RMASK 0xFF000000
			#define GMASK 0x00FF0000
			#define BMASK 0x0000FF00
			#define AMASK 0x000000FF
		#else
			#define AMASK 0xFF000000
			#define BMASK 0x00FF0000
			#define GMASK 0x0000FF00
			#define RMASK 0x000000FF
		#endif
	
		public:
			inline static SDL_Surface* createEmptySurface(signed short width, signed short height) {
				return SDL_CreateRGBSurface(0, width, height, 32, RMASK, GMASK, BMASK, AMASK);
			}
	};
};
