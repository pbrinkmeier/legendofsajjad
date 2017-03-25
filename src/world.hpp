#pragma once

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
			UI *m_ui;
			Player *m_player;
			Sprite *m_floor;
			std::vector<Penguin*> m_penguins;
	};
};
