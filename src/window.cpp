#include "window.hpp"
#include "keyboard.hpp"

#include <SDL2/SDL_image.h>
#include <iostream>

namespace los {
	Window::Window(const char *title, const signed short width, const signed short height)
		: WIDTH(width), HEIGHT(height) {

		const Uint32 W_FLAGS = SDL_WINDOW_ALLOW_HIGHDPI;
		m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, W_FLAGS);
		if (m_window == nullptr)
			std::cerr << "SRC: window.cpp\tERR: Failed to create window, " << SDL_GetError() << "\n";
	
		const Uint32 R_FLAGS = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
		m_renderer = SDL_CreateRenderer(m_window, -1, R_FLAGS);
		if (m_renderer == nullptr)
			std::cerr << "SRC: window.cpp\tERR: Failed to create renderer, " << SDL_GetError() << "\n";
	
		SDL_Surface *startScreen = IMG_Load("../res/screens/startscreen.png");
		SDL_Surface *endScreen = IMG_Load("../res/screens/endscreen.png");
		SDL_Surface *deathScreen = IMG_Load("../res/screens/deathscreen.png");

		m_startScreen = new Sprite(m_renderer, startScreen, 0, 0, width, height);
		m_endScreen = new Sprite(m_renderer, endScreen, 0, 0, width, height);
		m_deathScreen = new Sprite(m_renderer, deathScreen, 0, 0, width, height);

		SDL_FreeSurface(startScreen);
		SDL_FreeSurface(endScreen);
		SDL_FreeSurface(deathScreen);

		m_endTimer = 0;
		m_deathTimer = 0;
		m_startTimer = SDL_GetTicks() + 3000;
		m_shouldClose = false;
	}

	Window::~Window() {
		delete m_startScreen;
		delete m_endScreen;
		delete m_deathScreen;

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

		if (!m_gameStarted && m_startTimer < SDL_GetTicks()) {
			m_world = new World(m_renderer, WIDTH, HEIGHT);
			m_gameStarted = true;
		}

		if (m_gameStarted) {
			m_world->update(tslf, WIDTH, HEIGHT);
			
			if (m_world->gameWon())
				m_endTimer = SDL_GetTicks() + 3000;
			else if (m_world->gameOver())
				m_deathTimer = SDL_GetTicks() + 3000;
		}
	}

	void Window::render() {
		SDL_RenderClear(m_renderer);
		if (SDL_GetTicks() < m_startTimer)
			m_startScreen->render(m_renderer);

		else if (SDL_GetTicks() < m_endTimer)
			m_endScreen->render(m_renderer);

		else if (SDL_GetTicks() < m_deathTimer)
			m_deathScreen->render(m_renderer);

		bool running = m_gameStarted && !m_world->gameOver() && !m_world->gameWon();
		if (running && m_world != nullptr)
			m_world->render(m_renderer);
		SDL_RenderPresent(m_renderer);
	}
};
