#pragma once

#include "gfx/sprite.hpp"

#include <SDL2/SDL.h>

namespace los {
	class Player {
		public:
			Player(SDL_Renderer*, const float, const float);
			~Player();
		public:
			void render(SDL_Renderer*);
			void update(const double tslf);
		public:
			void hit();
			void resetProjectile();
		public:
			inline bool isInvincible() {
				return SDL_GetTicks() < m_invincibleUntil;
			}
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
			inline bool projectileFlying() {
				return m_projectileFlying;
			}
		public:
			inline SDL_Rect getProjectileHitbox() {
				SDL_Rect hitbox = {
					static_cast<signed short>(m_projectileSprite->getX()),
					static_cast<signed short>(m_projectileSprite->getY()),
					static_cast<signed short>(m_projectileSprite->getWidthResized()),
					static_cast<signed short>(m_projectileSprite->getHeightResized())
				};
				return hitbox;
			}
		private:
			bool m_dying = false;
			bool m_dead = false;
			Uint32 m_deathTimer;
		public:
			inline void die() {
				m_dying = true;
				m_deathTimer = SDL_GetTicks() + 3000;
			}
		public:
			inline bool isDying() {
				return m_dying;
			}
		public:
			inline bool isDead() {
				return m_dead;
			}
		private:
			Uint32 m_invincibleUntil = 0;
		private:
			unsigned char m_currentIndex = 0;
			Sprite **m_currentSprites = nullptr;
			Sprite **m_upSprites = new Sprite*[3];
			Sprite **m_downSprites = new Sprite*[3];
			Sprite **m_leftSprites = new Sprite*[3];
			Sprite **m_rightSprites = new Sprite*[3];
			Sprite *m_deathSprite;
		private:
			float m_projectileDirX = 0.0f;
			float m_projectileDirY = 0.0f;
			float m_projectilePosX = 0.0f;
			float m_projectilePosY = 0.0f;
			Uint32 m_nextLaunch = 0;
			bool m_projectileFlying = false;
			Sprite *m_projectileSprite;
		private:
			float m_posx;
			float m_posy;
			float m_width;
			float m_height;
			float m_widthResized;
			float m_heightResized;
	};
};
