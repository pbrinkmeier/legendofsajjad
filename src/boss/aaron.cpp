#include "aaron.hpp"

namespace los {
	Aaron::Aaron(SDL_Renderer *renderer) : Boss(renderer, 2, "../res/boss/aaron.png", 0, -300) {
		m_health = 600;
	}
	Aaron::~Aaron() {}
};
