#include "player.hpp"
#include "keyboard.hpp"
#include "util/surfaceEditor.hpp"
#include "util/surfaceCreator.hpp"

#include <SDL2/SDL_image.h>

namespace los {
	Player::Player(SDL_Renderer *renderer, const signed short posx, const signed short posy)
		: m_posx(posx), m_posy(posy) {
		SDL_Surface *sf = IMG_Load("../res/player.png");

		const signed short SIZEMOD = 3;
		m_width = 16;
		m_height = 16;
		m_widthResized = m_width * SIZEMOD;
		m_heightResized = m_height * SIZEMOD;
		
		for (unsigned char x = 0; x < 3; x++) {
			SDL_Surface *tmp = SurfaceEditor::crop(sf, 1 * m_width, x * m_height, m_width, m_height);
			m_upSprites[x] = new Sprite(renderer, tmp, m_posx, m_posy, m_widthResized, m_heightResized);
			SDL_FreeSurface(tmp);
		}

		for (unsigned char x = 0; x < 3; x++) {
			SDL_Surface *tmp = SurfaceEditor::crop(sf, 0 * m_width, x * m_height, m_width, m_height);
			m_downSprites[x] = new Sprite(renderer, tmp, m_posx, m_posy, m_widthResized, m_heightResized);
			SDL_FreeSurface(tmp);
		}

		for (unsigned char x = 0; x < 3; x++) {
			SDL_Surface *tmp = SurfaceEditor::crop(sf, 2 * m_width, x * m_height, m_width, m_height);
			m_rightSprites[x] = new Sprite(renderer, tmp, m_posx, m_posy, m_widthResized, m_heightResized);
			SDL_FreeSurface(tmp);
		}

		for (unsigned char x = 0; x < 3; x++) {
			SDL_Surface *tmp = SurfaceEditor::crop(sf, 3 * m_width, x * m_height, m_width, m_height);
			m_leftSprites[x] = new Sprite(renderer, tmp, m_posx, m_posy, m_widthResized, m_heightResized);
			SDL_FreeSurface(tmp);
		}

		SDL_Surface *projectile = IMG_Load("../res/projectile.png");
		m_projectileSprite = new Sprite(renderer, projectile, 0, 0, 8 * 1.5, 8 * 1.5);
		
		SDL_Surface *emptySurface = SurfaceCreator::createEmptySurface(16, 16);
		m_emptySprite = new Sprite(renderer, emptySurface, m_posx, m_posy, m_widthResized, m_heightResized);

		SDL_FreeSurface(emptySurface);
		SDL_FreeSurface(projectile);
		SDL_FreeSurface(sf);

		m_currentIndex = 0;
		m_currentSprites = m_downSprites;
	}

	Player::~Player() {
		delete m_emptySprite;
		delete[] m_upSprites;
		delete[] m_downSprites;
		delete[] m_leftSprites;
		delete[] m_rightSprites;
		delete[] m_projectileSprite;
	}

	void Player::render(SDL_Renderer *renderer) {
		if (isInvincible() && SDL_GetTicks() % 2 == 0) {
			m_emptySprite->setPosition(m_posx, m_posy);
			m_emptySprite->render(renderer);
		} else
			m_currentSprites[m_currentIndex]->render(renderer);

		if (m_projectileFlying)
			m_projectileSprite->render(renderer);
	}

	void Player::update(const double tslf) {
		static Uint32 lastTick = SDL_GetTicks();
		Uint32 thisTick = SDL_GetTicks();

		static unsigned char index = 0;
		bool walking = false;

		if (Keyboard::keyDown(SDL_SCANCODE_S)) {
			walking = true;
			m_currentSprites = m_downSprites;
		}
		else if (Keyboard::keyDown(SDL_SCANCODE_W)) {
			walking = true;
			m_currentSprites = m_upSprites;
		}
		else if (Keyboard::keyDown(SDL_SCANCODE_A)) {
			walking = true;
			m_currentSprites = m_leftSprites;
		}
		else if (Keyboard::keyDown(SDL_SCANCODE_D)) {
			walking = true;
			m_currentSprites = m_rightSprites;
		}

		if (Keyboard::keyDown(SDL_SCANCODE_RIGHT) && !m_projectileFlying && SDL_GetTicks() > m_nextLaunch) {
			m_projectileFlying = true;
			m_projectileDirY = 0.0f;
			m_projectileDirX = 1.0f;
			m_projectilePosX = m_posx + 48;
			m_projectilePosY = m_posy;
		}

		else if (Keyboard::keyDown(SDL_SCANCODE_LEFT) && !m_projectileFlying && SDL_GetTicks() > m_nextLaunch) {
			m_projectileFlying = true;
			m_projectileDirY = 0.0f;
			m_projectileDirX = -1.0f;
			m_projectilePosX = m_posx - 48;
			m_projectilePosY = m_posy;
		}

		else if (Keyboard::keyDown(SDL_SCANCODE_DOWN) && !m_projectileFlying && SDL_GetTicks() > m_nextLaunch) {
			m_projectileFlying = true;
			m_projectileDirY = 1.0f;
			m_projectileDirX = 0.0f;
			m_projectilePosX = m_posx;
			m_projectilePosY = m_posy + 48;
		}

		else if (Keyboard::keyDown(SDL_SCANCODE_UP) && !m_projectileFlying && SDL_GetTicks() > m_nextLaunch) {
			m_projectileFlying = true;
			m_projectileDirY = -1.0f;
			m_projectileDirX = 0.0f;
			m_projectilePosX = m_posx;
			m_projectilePosY = m_posy - 48;
		}

		if (m_projectileFlying) {
			m_projectilePosX += m_projectileDirX * tslf * 1024;
			m_projectilePosY += m_projectileDirY * tslf * 1024;

			if (m_projectilePosX < -100 || m_projectilePosX > 900 || m_projectilePosY < -100 || m_projectilePosY > 700)
				m_projectileFlying = false;

			m_projectileSprite->setPosition(m_projectilePosX, m_projectilePosY);
		}

		if (walking && thisTick - lastTick > 200) {
			index++;
			index = index % 4;
			lastTick = thisTick;
			switch (index) {
				case 0: m_currentIndex = 0; break;
				case 1: m_currentIndex = 1; break;
				case 2: m_currentIndex = 0; break;
				case 3: m_currentIndex = 2; break;
				default: m_currentIndex = 0; break;
			}
		}

		if (!walking) {
			index = 0;
			m_currentIndex = 0;
		}
	}

	void Player::hit() {
		m_invincibleUntil = SDL_GetTicks() + 2000;
	}

	void Player::resetProjectile() {
		m_projectileFlying = false;
		m_nextLaunch = SDL_GetTicks() + 100;
	}
};
