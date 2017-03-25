#include "ui.hpp"
#include "util/surfaceEditor.hpp"

#include <iostream>
#include <SDL2/SDL_image.h>

namespace los {
	UI::UI(SDL_Renderer *renderer) {
		SDL_Surface *hrts = IMG_Load("../res/hearts.png");
		const signed short SIZEMOD = 3;

		for (unsigned char x = 0; x < 2; x++) {
			SDL_Surface *tmp = SurfaceEditor::crop(hrts, x * 8, 0, 8, 8);
			m_hearts[x] = new Sprite(renderer, tmp, 0, 32, 10 * SIZEMOD, 10 * SIZEMOD);
			SDL_FreeSurface(tmp);
		}

		SDL_FreeSurface(hrts);
	}

	UI::~UI() {
		delete[] m_hearts;
	}

	void UI::render(SDL_Renderer *renderer) {	
		for (signed char i = 0; i < m_heartCount; i++) {
			m_hearts[0]->setPosition(i * 32 + 32, m_hearts[0]->getY());
			m_hearts[0]->render(renderer);
		}

		for (signed char i = m_heartCount; i < HEART_MAX; i++) {
			m_hearts[1]->setPosition(i * 32 + 32, m_hearts[0]->getY());
			m_hearts[1]->render(renderer);
		}
	}
		
	void UI::setPlayerHealth(signed short h) {
		m_heartCount = h;
	
		if (m_heartCount < 0)
			m_heartCount = 0;
	}

	short UI::getPlayerHealth() {
		return m_heartCount;
	}
		
	void UI::setPenguinDeaths(signed short i) {
		m_penguinDeaths = i;
	}

	short UI::getPenguinDeaths() {
		return m_penguinDeaths;
	}
};
