#pragma once

#include "../gfx/sprite.hpp"

#include <SDL2/SDL.h>

namespace los {
	class Boss {
		public:
			Boss(SDL_Renderer*, const char*, float, float);
			virtual ~Boss();
		public:
			void render(SDL_Renderer*);
			virtual void update(const double, const float, const float);
		protected:
			Sprite *m_boss;
			Sprite *m_announcement;
		protected:
			Uint32 m_animStart;
			Uint32 m_animFinish;
	};
};
