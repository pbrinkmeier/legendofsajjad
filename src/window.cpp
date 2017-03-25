#include "window.hpp"
#include "keyboard.hpp"

#include <iostream>

namespace los {
	Window::Window(const char *title, const signed short width, const signed short height)
		: WIDTH(width), HEIGHT(height) {

		const Uint32 W_FLAGS = SDL_WINDOW_ALLOW_HIGHDPI;
		m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, W_FLAGS);
		if (m_window == nullptr)
			std::cerr << "SRC: window.cpp\t ERR: Failed to create window, " << SDL_GetError() << "\n";
	
		const Uint32 R_FLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
		m_renderer = SDL_CreateRenderer(m_window, -1, R_FLAGS);
		if (m_renderer == nullptr)
			std::cerr << "SRC: window.cpp\t ERR: Failed to create renderer, " << SDL_GetError() << "\n";
	
		m_world = new World(m_renderer, width, height);
		m_shouldClose = false;
	}

	Window::~Window() {
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
	}

	void Window::update() {
		static Uint32 lastTick = SDL_GetTicks();
		Uint32 thisTick = SDL_GetTicks();
		double tslf = (thisTick - lastTick) / 1000.0;
		lastTick = thisTick;

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT || Keyboard::keyDown(SDL_SCANCODE_ESCAPE))
				m_shouldClose = true;
		}

		m_world->update(tslf, WIDTH, HEIGHT);
	}

	void Window::render() {
		m_world->render(m_renderer);
	}
};
