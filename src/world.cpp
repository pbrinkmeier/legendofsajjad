#include "world.hpp"
#include "keyboard.hpp"

#include "util/surfaceEditor.hpp"
#include "util/surfaceCreator.hpp"

#include <stdlib.h>

#include <iostream>
#include <SDL2/SDL_image.h>

namespace los {
	World::World(SDL_Renderer *renderer, signed short windowWidth, signed short windowHeight) {	
		SDL_Surface **floorPixels = new SDL_Surface*[4];
		SDL_Surface *floorPNG = IMG_Load("../res/floor.png");

		srand(time(NULL));
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

		m_ui = new UI(renderer);
		m_player = new Player(renderer, (windowWidth - 16 * 3) / 2.0, (windowHeight - 24 * 3) / 2.0);
		
		const signed short MAX_PENGUINS = 20;
		for (signed short i = 0; i < MAX_PENGUINS; i++) {
			signed short sWidth = static_cast<signed short>(m_floor->getWidthResized() - windowWidth);
			signed short sHeight = static_cast<signed short>(m_floor->getHeightResized() - windowHeight);
			float pposx = rand() % sWidth - sWidth / 2 + windowWidth / 2;
			float pposy = rand() % sHeight - sHeight / 2 + windowHeight / 2;
			Penguin *pen = new Penguin(renderer, pposx, pposy);
			m_penguins.push_back(pen);
		}
	}

	World::~World() {
		for (Penguin *p : m_penguins)
			delete p;
		m_penguins.clear();

		delete m_ui;
		delete m_player;
		delete m_floor;
	}

	void World::render(SDL_Renderer *m_renderer) {
		SDL_RenderClear(m_renderer);

		m_floor->render(m_renderer);
		for (Penguin *p : m_penguins)
			p->render(m_renderer);
		m_player->render(m_renderer);
		m_ui->render(m_renderer);

		SDL_RenderPresent(m_renderer);
	}

	void World::update(const double tslf, signed short windowWidth, signed short windowHeight) {
		const float SPEED = tslf * 150.0f;
		float xOff = 0.0f;
		float yOff = 0.0f;

		if (Keyboard::keyDown(SDL_SCANCODE_W))
			yOff += SPEED;
		if (Keyboard::keyDown(SDL_SCANCODE_S))
			yOff -= SPEED;
		if (Keyboard::keyDown(SDL_SCANCODE_A))
			xOff += SPEED;
		if (Keyboard::keyDown(SDL_SCANCODE_D))
			xOff -= SPEED;

		if (m_floor->getX() >= -0.1f && xOff > 0.0f)
			xOff = 0.0f;
		if (m_floor->getY() >= -0.1f && yOff > 0.0f)
			yOff = 0.0f;
		if (m_floor->getX() <= -m_floor->getWidth() - windowWidth && xOff < 0.0f)
			xOff = 0.0f;
		if (m_floor->getY() <= -m_floor->getHeight() - windowHeight && yOff < 0.0f)
			yOff = 0.0f;

		m_floor->move(xOff, yOff);
		m_player->update(tslf);
		
		auto it = m_penguins.begin();
		while (it != m_penguins.end()) {
			(*it)->update(tslf, xOff, yOff, windowWidth, windowHeight);
			
			if ((*it)->isDead())
				it = m_penguins.erase(it);
			else ++it;
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
					m_player->hit();
					m_ui->setPlayerHealth(m_ui->getPlayerHealth() - 1);
				}
			}

			SDL_Rect projectileHitbox = m_player->getProjectileHitbox();
			if (m_player->projectileFlying() && SDL_HasIntersection(&penBox, &projectileHitbox) == SDL_TRUE) {
				m_player->resetProjectile();
				m_ui->setPenguinDeaths(m_ui->getPenguinDeaths() + 1);
				(*it)->hit();
			} else ++it;
		}
	}
};
