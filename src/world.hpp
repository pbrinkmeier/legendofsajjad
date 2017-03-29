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
		public:
			inline bool gameWon() {
				return m_won;
			}
		public:
			inline bool gameOver() {
				return m_lost;
			}
		private:
			void applyCollisions();
		private:
			signed char m_nextBossID;
			Uint32 m_bossSpawnTime;
		private:
			bool m_lost = false;
			bool m_won = false;
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
