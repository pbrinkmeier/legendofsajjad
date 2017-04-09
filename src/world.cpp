#include "world.hpp"
#include "keyboard.hpp"

#include "sfx/soundBank.hpp"
#include "util/surfaceEditor.hpp"
#include "util/surfaceCreator.hpp"

#include "boss/patrick.hpp"
#include "boss/aaron.hpp"
#include "boss/valentin.hpp"
#include "boss/jannik.hpp"

#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <SDL2/SDL_image.h>

namespace los {
	World::World(SDL_Renderer *renderer, signed short windowWidth, signed short windowHeight) 
		: m_renderer(renderer) {
		SDL_Surface **floorPixels = new SDL_Surface*[4];
		SDL_Surface *floorPNG = IMG_Load("../res/tiles/floor.png");

		srand(static_cast<unsigned int>(time(NULL)));
		floorPixels[0] = SurfaceEditor::crop(floorPNG, 0,  0, 16, 16);
		floorPixels[1] = SurfaceEditor::crop(floorPNG, 16, 0, 16, 16);
		floorPixels[2] = SurfaceEditor::crop(floorPNG, 32, 0, 16, 16);
		floorPixels[3] = SurfaceEditor::crop(floorPNG, 48, 0, 16, 16);
		
		SDL_Surface *floorSurface = SurfaceCreator::createEmptySurface(windowWidth, windowHeight);
		SDL_Rect dst = { 0, 0, static_cast<int>(floorPixels[0]->w), static_cast<int>(floorPixels[0]->h) };
		
		for (signed short y = 0; y < windowHeight / floorPixels[0]->h + 1; y++) {
			for (signed short x = 0; x < windowWidth / floorPixels[0]->w + 1; x++) {
				dst.x = x * floorPixels[0]->w;
				dst.y = y * floorPixels[0]->h;

				short index = rand() % 16;
				if (index > 3)
					index = 0;
				SDL_BlitSurface(floorPixels[index], NULL, floorSurface, &dst);
			}
		}

		const float wResized = windowWidth * 3.0f;
		const float hResized = windowHeight * 3.0f;
		const float fposx = -(wResized - windowWidth) / 2.0f;
		const float fposy = -(hResized - windowHeight) / 2.0f;
		m_floor = new Sprite(renderer, floorSurface, fposx, fposy, wResized, hResized);
		SDL_FreeSurface(floorSurface);

		for (signed char i = 0; i < 4; i++)
			SDL_FreeSurface(floorPixels[i]);
		SDL_FreeSurface(floorPNG);

		SoundBank::init();
		m_ui = new UI(renderer);
		m_player = new Player(renderer, (windowWidth - 16 * 3) / 2.0f, (windowHeight - 24 * 3) / 2.0f);
		
		const signed short MAX_PENGUINS = 32;
		for (signed short i = 0; i < MAX_PENGUINS; i++) {
			const signed short sWidth = static_cast<signed short>(m_floor->getWidthResized() - windowWidth);
			const signed short sHeight = static_cast<signed short>(m_floor->getHeightResized() - windowHeight);
			const float pposx = static_cast<float>(rand() % sWidth - sWidth / 2 + windowWidth / 2);
			const float pposy = static_cast<float>(rand() % sHeight - sHeight / 2 + windowHeight / 2);
			Penguin *pen = new Penguin(renderer, pposx, pposy);
			m_penguins.push_back(pen);
		}

		SoundBank::loop(SoundBank::SFX_MAINTHEME);
		m_nextBossID = 0;
		m_bossSpawnTime = SDL_GetTicks() + 10000;
	}

	World::~World() {
		for (Penguin *p : m_penguins)
			delete p;
		m_penguins.clear();

		SoundBank::destroy();

		delete m_ui;
		delete m_player;
		delete m_floor;
	}

	void World::render(SDL_Renderer *renderer) {
		m_floor->render(renderer);
		for (Penguin *p : m_penguins)
			p->render(renderer);
		m_player->render(renderer);

		if (m_boss != nullptr)
			m_boss->render(renderer);

		m_ui->render(renderer);
	}

	void World::update(const double tslf, signed short windowWidth, signed short windowHeight) {
		const float SPEED = static_cast<float>(tslf * 150.0f);
		float xOff = 0.0f;
		float yOff = 0.0f;
		
		if (m_ui->getPlayerHealth() <= 3 && !m_player->isDying() && !m_player->isDead())
			m_player->die();

		if (m_player->isDead())
			m_lost = true;

		if (!m_player->isDying()) {
			if (Keyboard::keyDown(SDL_SCANCODE_W))
				yOff += SPEED;
			if (Keyboard::keyDown(SDL_SCANCODE_S))
				yOff -= SPEED;
			if (Keyboard::keyDown(SDL_SCANCODE_A))
				xOff += SPEED;
			if (Keyboard::keyDown(SDL_SCANCODE_D))
				xOff -= SPEED;

			if (m_floor->getX() >= -3.0f && xOff > 0.0f)
				xOff = 0.0f;
			if (m_floor->getY() >= -3.0f && yOff > 0.0f)
				yOff = 0.0f;
			if (m_floor->getX() <= -m_floor->getWidth() - windowWidth + 3.0f && xOff < 0.0f)
				xOff = 0.0f;
			if (m_floor->getY() <= -m_floor->getHeight() - windowHeight + 3.0f && yOff < 0.0f)
				yOff = 0.0f;
		}

		m_floor->move(xOff, yOff);
		m_player->update(tslf);
	
		if (m_boss != nullptr)
			m_boss->update(tslf, m_floor->getX(), m_floor->getY(), xOff, yOff);

		if (SDL_GetTicks() > m_bossSpawnTime)
			for (Penguin *pen : m_penguins)
				pen->hit();

		auto it = m_penguins.begin();
		while (it != m_penguins.end()) {
			(*it)->update(tslf, xOff, yOff, windowWidth, windowHeight);
			
			if ((*it)->isDead())
				it = m_penguins.erase(it);
			else ++it;
		}
		
		if (m_penguins.size() == 0 && m_boss == nullptr) {
			switch (m_nextBossID) {
				case 0:
					m_boss = new Patrick(m_renderer);
					break;
				case 1:
					m_boss = new Valentin(m_renderer);
					break;
				case 2:
					m_boss = new Aaron(m_renderer);
					break;
				case 3:
					m_boss = new Jannik(m_renderer);
					SoundBank::stop();
					SoundBank::loop(SoundBank::SFX_FINALBOSS);
					break;
				default:
					m_boss = nullptr;
			}
		}
		
		if (m_boss != nullptr) {
			if (m_boss->isDead()) {
				if (m_boss->IDENTIFIER == 3) {
					m_won = true;
					return;
				}

				for (signed char i = 0; i < 4 - m_boss->IDENTIFIER; i++) {
					for (signed char j = 0; j < 32; j++) {
						signed short sWidth = static_cast<signed short>(m_floor->getWidthResized() - windowWidth);
						signed short sHeight = static_cast<signed short>(m_floor->getHeightResized() - windowHeight);
						const float pposx = static_cast<float>(rand() % sWidth - sWidth / 2 + windowWidth / 2);
						const float pposy = static_cast<float>(rand() % sHeight - sHeight / 2 + windowHeight / 2);
						Penguin *pen = new Penguin(m_renderer, pposx, pposy);
						m_penguins.push_back(pen);
					}
				}
				SoundBank::playSound(SoundBank::SFX_BOSSDEATH);
				m_bossSpawnTime = SDL_GetTicks() + 10000;
				delete m_boss;
				m_boss = nullptr;
				m_nextBossID++;
			}
		}
	
		applyCollisions();
	}

	void World::applyCollisions() {
		SDL_Rect playerBox = m_player->getHitbox();
		auto it = m_penguins.begin();
		while (it != m_penguins.end()) {
			if ((*it)->isDying()) {
				++it;
				continue;
			}

			SDL_Rect penBox = (*it)->getHitbox();
			if (SDL_HasIntersection(&playerBox, &penBox) == SDL_TRUE) {
				if (!m_player->isInvincible()) {
					SoundBank::playSound(SoundBank::SFX_PLAYERDMG);
					m_player->hit();
					m_ui->setPlayerHealth(m_ui->getPlayerHealth() - 4);
				}
			}

			SDL_Rect projectileHitbox = m_player->getProjectileHitbox();
			if (m_player->projectileFlying() && SDL_HasIntersection(&penBox, &projectileHitbox) == SDL_TRUE) {
				SoundBank::playSound(SoundBank::SFX_PENGUIN);
				m_player->resetProjectile();
				m_ui->setPlayerHealth(m_ui->getPlayerHealth() + 1);
				m_ui->setPenguinDeaths(m_ui->getPenguinDeaths() + 1);
				(*it)->hit();
			} else ++it;
		}

		if (m_boss != nullptr) {
			SDL_Rect bossBox = m_boss->getHitbox();
			SDL_Rect playerBox = m_player->getHitbox();
			SDL_Rect projectileBox = m_player->getProjectileHitbox();
			if (!m_boss->isInvincible() && m_player->projectileFlying())
				if (SDL_HasIntersection(&bossBox, &projectileBox) == SDL_TRUE) {
					m_boss->hit();
					m_player->resetProjectile();
					SoundBank::playSound(SoundBank::SFX_PATRICK);
				}

			if (!m_player->isInvincible())
				if (SDL_HasIntersection(&bossBox, &playerBox) == SDL_TRUE) {
					m_player->hit();

					signed char damageMultiplier = 1;
					if (m_boss->IDENTIFIER > 2)
						damageMultiplier = m_boss->IDENTIFIER;

					m_ui->setPlayerHealth(m_ui->getPlayerHealth() - 4 * damageMultiplier);
					SoundBank::playSound(SoundBank::SFX_PLAYERDMG);
				}
		}
	}
};
