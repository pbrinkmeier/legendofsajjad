#include "boss.hpp"

#include "../util/surfaceEditor.hpp"

#include <SDL2/SDL_image.h>
#include <iostream>

namespace los {
	Boss::Boss(SDL_Renderer *renderer, const char *spritePath, const float posx, const float posy) {
		SDL_Surface *sprite = IMG_Load(spritePath);
		if (sprite == nullptr)
			std::cerr << "SRC: boss.cpp\tERR: Failed to load sprite " << spritePath << "\n";

		if (sprite->w != 160 || sprite->h != 80)
			std::cerr << "SRC: boss.cpp\tERR: Sprite " << spritePath << " has incorrect dimensions\n";

		SDL_Surface *boss = SurfaceEditor::crop(sprite, 0, 0, 48, 48);
		SDL_Surface *annc = SurfaceEditor::crop(sprite, 0, 48, 160, 32);

		const float anPosx = (800 - 160 * 3) / 2;
		const float anPosy = 600;

		m_boss = new Sprite(renderer, boss, posx, posy, boss->w * 6, boss->h * 6);
		m_announcement = new Sprite(renderer, annc, anPosx, anPosy, annc->w * 3, annc->h * 3);

		m_animStart = SDL_GetTicks();
		m_animFinish = m_animStart + 5000;

		SDL_FreeSurface(sprite);
		SDL_FreeSurface(boss);
		SDL_FreeSurface(annc);
	}

	Boss::~Boss() {
		delete m_boss;
		delete m_announcement;
	}

	void Boss::render(SDL_Renderer *renderer) {
		m_boss->render(renderer);

		if (SDL_GetTicks() < m_animFinish)
			m_announcement->render(renderer);
	}

	void Boss::update(const double tslf, const float xOffset, const float yOffset) {
		m_boss->move(xOffset, yOffset);
		
		if (SDL_GetTicks() > m_animStart && SDL_GetTicks() < m_animStart + 1000)
			m_announcement->move(0, -tslf * 120);

		if (SDL_GetTicks() > m_animStart + 4000 && SDL_GetTicks() < m_animFinish)
			m_announcement->move(0, tslf * 120);
	}
};
