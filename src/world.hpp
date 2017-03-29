#pragma once

#include "boss/boss.hpp"
#include "gfx/sprite.hpp"
#include "player.hpp"
#include "penguin.hpp"
#include "ui.hpp"

#include <vector>
#include <SDL2/SDL.h>

namespace los {
	class World {
		public:
			World(SDL_Renderer*, signed short, signed short);
			~World();
		public:
			void render(SDL_Renderer*);
			void update(const double, signed short, signed short);
		private:
			void applyCollisions();
		private:
			signed char m_nextBossID;
			Uint32 m_bossSpawnTime;
		private:
			SDL_Renderer *m_renderer;
		private:
			UI *m_ui = nullptr;
			Boss *m_boss = nullptr;
			Player *m_player = nullptr;
			Sprite *m_floor = nullptr;
			std::vector<Penguin*> m_penguins;
	};
};
