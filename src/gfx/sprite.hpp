#pragma once

#include <SDL2/SDL.h>

namespace los {
	class Sprite {
		public:
			Sprite(SDL_Renderer*, const char*, const float, const float, const float, const float);
			Sprite(SDL_Renderer*, const SDL_Surface*, const float, const float, const float, const float);
			~Sprite();
		public:
			void render(SDL_Renderer*);
		private:
			float m_width = 0.0f;
			float m_height = 0.0f;
		public:
			inline float getWidth() const { return m_width; }
			inline float getHeight() const { return m_height; }
			inline float getWidthResized() const { return m_widthResized; }
			inline float getHeightResized() const { return m_heightResized; }
			inline float getX() const { return m_posx; }
			inline float getY() const { return m_posy; }
		public:
			inline void setPosition(float x, float y) {
				m_posx = x;
				m_posy = y;
			}
		public:
			inline void move(float dx, float dy) {
				m_posx += dx;
				m_posy += dy;
			}
		private:
			float m_posx = 0.0f;
			float m_posy = 0.0f;
		private:
			float m_widthResized = 0;
			float m_heightResized = 0;
		private:
			SDL_Surface *m_surface = nullptr;
			SDL_Texture *m_texture = nullptr;
	};
};
