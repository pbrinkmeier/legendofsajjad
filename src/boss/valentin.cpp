#include "valentin.hpp"

namespace los {
	Valentin::Valentin(SDL_Renderer *renderer) : Boss(renderer, 1, "../res/boss/valentin.png", 0, -300) {}
	Valentin::~Valentin() {}
};
