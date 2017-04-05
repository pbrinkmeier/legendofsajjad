#include "penguin.hpp"

#include "keyboard.hpp"
#include "util/surfaceEditor.hpp"
#include "util/surfaceCreator.hpp"

#include <ctime>
#include <stdlib.h>
#include <iostream>
#include <SDL2/SDL_image.h>

namespace los {
	Penguin::Penguin(SDL_Renderer *renderer, const float posx, const float posy)
		: m_posx(posx), m_posy(posy) {
		SDL_Surface *sf = IMG_Load("../res/entities/tux.png");

		const signed short SIZEMOD = 3;
		m_width = 16;
		m_height = 16;
		m_widthResized = m_width * SIZEMOD;
		m_heightResized = m_height * SIZEMOD;
		
		const signed short W = static_cast<signed short>(std::floor(m_width));
		const signed short H = static_cast<signed short>(std::floor(m_height));
		for (unsigned char x = 0; x < 3; x++) {
			const signed short px = static_cast<signed short>(1 * m_width);
			const signed short py = static_cast<signed short>(x * m_height);
			SDL_Surface *tmp = SurfaceEditor::crop(sf, px, py, W, H);
			m_upSprites[x] = new Sprite(renderer, tmp, m_posx, m_posy, m_widthResized, m_heightResized);
			SDL_FreeSurface(tmp);
		}

		for (unsigned char x = 0; x < 3; x++) {
			const signed short px = 0;
			const signed short py = static_cast<signed short>(x * m_height);
			SDL_Surface *tmp = SurfaceEditor::crop(sf, px, py, W, H);
			m_downSprites[x] = new Sprite(renderer, tmp, m_posx, m_posy, m_widthResized, m_heightResized);
			SDL_FreeSurface(tmp);
		}

		for (unsigned char x = 0; x < 3; x++) {
			const signed short px = static_cast<signed short>(2 * m_width);
			const signed short py = static_cast<signed short>(x * m_height);
			SDL_Surface *tmp = SurfaceEditor::crop(sf, px, py, W, H);
			m_rightSprites[x] = new Sprite(renderer, tmp, m_posx, m_posy, m_widthResized, m_heightResized);
			SDL_FreeSurface(tmp);
		}

		for (unsigned char x = 0; x < 3; x++) {
			const signed short px = static_cast<signed short>(3 * m_width);
			const signed short py = static_cast<signed short>(x * m_height);
			SDL_Surface *tmp = SurfaceEditor::crop(sf, px, py, W, H);
			m_leftSprites[x] = new Sprite(renderer, tmp, m_posx, m_posy, m_widthResized, m_heightResized);
			SDL_FreeSurface(tmp);
		}

		SDL_Surface *deathCropped = SurfaceEditor::crop(sf, rand() % 2 * W, 3 * H, W, H);
		SDL_Surface *emptySurface = SurfaceCreator::createEmptySurface(W, H);
		m_deathSprites[0] = new Sprite(renderer, deathCropped, m_posx, m_posy, m_widthResized, m_heightResized);
		m_deathSprites[1] = new Sprite(renderer, emptySurface, m_posx, m_posy, m_widthResized, m_heightResized);
		SDL_FreeSurface(emptySurface);
		SDL_FreeSurface(deathCropped);

		m_currentIndex = 0;
		m_currentSprites = m_downSprites;
	}

	Penguin::~Penguin() {
		delete[] m_upSprites;
		delete[] m_downSprites;
		delete[] m_leftSprites;
		delete[] m_rightSprites;
		delete[] m_deathSprites;
	}

	void Penguin::render(SDL_Renderer *renderer) {
		if (m_isDying) {
			short index = SDL_GetTicks() % 2;
			m_deathSprites[index]->setPosition(m_posx, m_posy);
			m_deathSprites[index]->render(renderer);
			return;
		}
		
		m_currentSprites[m_currentIndex]->render(renderer);
	}

	void Penguin::update(const double tslf, float xOffset, float yOffset, signed short windowW, signed short windowH) {
		if (m_isDying) {
			if (m_deathTimer == 0)
				m_deathTimer = SDL_GetTicks() + 3000;
		
			if (m_deathTimer < SDL_GetTicks())
				m_isDead = true;

			m_posx += xOffset;
			m_posy += yOffset;
			return;
		}
		
		Uint32 thisTick = SDL_GetTicks();

		if (thisTick > m_nextTick) {
			const signed char rnr = rand() % 4;
			if (rnr == 0) {
				m_posxDir = static_cast<float>(rand() % 3 - 1);
				m_posyDir = 0.0f;	
			} else if (rnr == 1) {
				m_posxDir = 0.0f;	
				m_posyDir = static_cast<float>(rand() % 3 - 1);
			} else {
				m_posxDir = 0.0f;
				m_posyDir = 0.0f;
			}
			
			m_nextTick = thisTick + rand() % 5000;
		}

		bool walking = false;
		if (m_posyDir > 0 && abs(m_posyDir) > abs(m_posxDir)) {
			walking = true;
			m_currentSprites = m_downSprites;
		}
		else if (m_posyDir < 0 && abs(m_posyDir) > abs(m_posxDir)) {
			walking = true;
			m_currentSprites = m_upSprites;
		}
		else if (m_posxDir < 0 && abs(m_posyDir) < abs(m_posxDir)) {
			walking = true;
			m_currentSprites = m_leftSprites;
		}
		else if (m_posxDir > 0 && abs(m_posyDir) < abs(m_posxDir)) {
			walking = true;
			m_currentSprites = m_rightSprites;
		}

		if (walking && thisTick - m_lastWalkTick > 200) {
			m_index++;
			m_index = m_index % 4;
			m_lastWalkTick = thisTick;
			switch (m_index) {
				case 0: m_currentIndex = 0; break;
				case 1: m_currentIndex = 1; break;
				case 2: m_currentIndex = 0; break;
				case 3: m_currentIndex = 2; break;
				default: m_currentIndex = 0; break;
			}
		}

		if (!walking) {
			m_index = 0;
			m_currentIndex = 0;
		}

		const signed short DETECTDIFF = 256;
		if (abs(m_posx - (windowW - 48) / 2) < DETECTDIFF && abs(m_posy - (windowH - 72) / 2) < DETECTDIFF) {
			float playerDX = (windowW - 48) / 2.0f - m_posx;
			float playerDY = (windowH - 48) / 2.0f - m_posy;

			m_posxDir = playerDX / 130.0f;
			m_posyDir = playerDY / 130.0f;

			m_following = true;
		} else m_following = false;

		m_posx += static_cast<float>(m_posxDir * tslf * 80.0f + xOffset);
		m_posy += static_cast<float>(m_posyDir * tslf * 80.0f + yOffset);
		m_currentSprites[m_currentIndex]->setPosition(m_posx, m_posy);
	}
};
