#include "ui.hpp"
#include "util/surfaceEditor.hpp"

#include <cmath>
#include <iostream>
#include <SDL2/SDL_image.h>

namespace los {
	UI::UI(SDL_Renderer *renderer) {
		SDL_Surface *hrt = IMG_Load("../res/misc/heart.png");
		const signed short SIZEMOD = 3;
		m_heart = new Sprite(renderer, hrt, 0, 32, 10 * SIZEMOD, 10 * SIZEMOD);
		SDL_FreeSurface(hrt);
	}

	UI::~UI() {
		delete m_heart;
	}

	void UI::render(SDL_Renderer *renderer) {	
		for (signed char i = 0; i < m_heartCount; i++) {
			m_heart->setPosition(i * 32 + 32, m_heart->getY());
			m_heart->render(renderer);
		}
	}
		
	void UI::setPlayerHealth(signed short h) {
		m_playerHealth = h;
		m_heartCount = std::floor(h / 4);
		
		if (m_heartCount > 64) {
			m_playerHealth = 64 * 4;
			m_heartCount = 64;
		}

		if (m_heartCount < 0) {
			m_playerHealth = 64 * 4;
			m_heartCount = 0;
		}
	}

	short UI::getPlayerHealth() {
		return m_playerHealth;
	}
		
	void UI::setPenguinDeaths(signed short i) {
		m_penguinDeaths = i;
	}

	short UI::getPenguinDeaths() {
		return m_penguinDeaths;
	}
};
