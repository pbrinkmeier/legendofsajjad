#pragma once

#include "world.hpp"
#include <SDL2/SDL.h>

namespace los {
	class Window {
		public:
			Window(const char*, const signed short, const signed short);
			~Window();
		public:
			void update();
			void render();
		public:
			inline bool shouldClose() const {
				return m_shouldClose;
			}
		public:
			const signed short WIDTH;
			const signed short HEIGHT;
		private:
			bool m_shouldClose = true;
			SDL_Window *m_window = nullptr;
			SDL_Renderer *m_renderer = nullptr;
		private:
			World *m_world = nullptr;
	};
};
