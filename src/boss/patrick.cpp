#include "patrick.hpp"

#include <random>

namespace los {
	Patrick::Patrick(SDL_Renderer *renderer) : Boss(renderer, "../res/boss/patrick.png", 0, 0) {}
	Patrick::~Patrick() {}

	void Patrick::update(const double tslf, const float xOffset, const float yOffset) {
		Boss::update(tslf, xOffset, yOffset);

		static float xDir = rand() % 2;
		static float yDir = rand() % 2;
		static Uint32 nextUpdate = SDL_GetTicks() + 3000;

		if (SDL_GetTicks() > nextUpdate) {
			xDir = rand() % 2;
			yDir = rand() % 2;
			nextUpdate = SDL_GetTicks() + 3000;
		}

		m_boss->move(xDir * tslf * 100, yDir * tslf * 100);
	}
};
