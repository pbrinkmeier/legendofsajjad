#include "window.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifdef main
	#undef main
#endif

using namespace los;

int main(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		std::cerr << "SRC: main.cpp\tERR: Failed to initialize SDL2\n";

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
		std::cerr << "SRC: main.cpp\tERR: Failed to initialize SDL2_image\n";

	Window window("Legend Of Sajjad", 800, 600);
	while (!window.shouldClose()) {
		window.render();
		window.update();
	}

	IMG_Quit();
	SDL_Quit();
	return 0;
}
