#pragma once

#include "gfx/sprite.hpp"

namespace los {
	class Penguin {
		public:
			Penguin(SDL_Renderer*, const signed short, const signed short);
			~Penguin();
		public:
			void render(SDL_Renderer*);
			void update(const double tslf, float, float, signed short, signed short);
		public:
			const SDL_Rect getHitbox() {
				SDL_Rect hitbox = {
					static_cast<signed short>(m_posx),
					static_cast<signed short>(m_posy),
					static_cast<signed short>(m_widthResized),
					static_cast<signed short>(m_heightResized)
				};
				return hitbox;
			}
		public:
			inline void hit() { m_isDying = true; }
			inline bool isDead() { return m_isDead; }
			inline bool isDying() { return m_isDying; }
		private:
			unsigned char m_currentIndex = 0;
			Sprite **m_currentSprites = nullptr;
			Sprite **m_upSprites = new Sprite*[3];
			Sprite **m_downSprites = new Sprite*[3];
			Sprite **m_leftSprites = new Sprite*[3];
			Sprite **m_rightSprites = new Sprite*[3];
			Sprite **m_deathSprites = new Sprite*[2];
		private:
			signed char m_index = 0;
			bool m_following = false;
			Uint32 m_lastWalkTick = SDL_GetTicks();
		private:
			float m_posxDir = 0.0f;
			float m_posyDir = 0.0f;
			Uint32 m_nextTick = SDL_GetTicks();
		private:
			bool m_isDying = false;
			bool m_isDead = false;
			Uint32 m_deathTimer = 0;
		private:
			float m_posx;
			float m_posy;
			float m_width;
			float m_height;
			float m_widthResized;
			float m_heightResized;
	};
};
