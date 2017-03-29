#include "patrick.hpp"

namespace los {
	Patrick::Patrick(SDL_Renderer *renderer) : Boss(renderer, 0, "../res/boss/patrick.png", 0, -300) {}
	Patrick::~Patrick() {}
};
