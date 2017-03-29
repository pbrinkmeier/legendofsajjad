#include "jannik.hpp"

namespace los {
	Jannik::Jannik(SDL_Renderer *renderer) : Boss(renderer, 3, "../res/boss/jannik.png", 0, -300) {
		m_health = 1500;
	}
	Jannik::~Jannik() {}
};
