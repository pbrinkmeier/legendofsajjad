#pragma once

#include "../gfx/sprite.hpp"

#include <SDL2/SDL.h>

namespace los {
	class Boss {
		public:
			Boss(SDL_Renderer*, const signed char, const char*, float, float);
			virtual ~Boss();
		public:
			void render(SDL_Renderer*);
			virtual void update(const double, const float, const float, const float, const float);
		public:
			void hit();
		public:
			inline bool isDead() { return m_health <= 0; }
		public:
			inline SDL_Rect getHitbox() {
				SDL_Rect hitbox = {
					static_cast<signed int>(m_boss->getX()),
					static_cast<signed int>(m_boss->getY()),
					static_cast<signed int>(m_boss->getWidthResized()),
					static_cast<signed int>(m_boss->getHeightResized())
				};
				return hitbox;
			}
		public:
			inline Uint32 isInvincible() { return SDL_GetTicks() < m_invincibleUntil; }
		public:
			const signed char IDENTIFIER;
		protected:
			signed short m_health;
		protected:
			Uint32 m_invincibleUntil = 0;
		protected:
			Sprite *m_boss;
			Sprite *m_announcement;
		protected:
			Uint32 m_animStart;
			Uint32 m_animFinish;
	};
};
