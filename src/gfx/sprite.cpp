#include "sprite.hpp"

#include <SDL2/SDL_image.h>
#include <iostream>

namespace los {
	Sprite::Sprite(SDL_Renderer *rn, const char *path, const float x, const float y, const float w, const float h)
		: m_posx(x), m_posy(y), m_widthResized(w), m_heightResized(h) {
		m_surface = IMG_Load(path);
		if (m_surface == nullptr)
			std::cerr << "SRC: sprite.cpp\t ERR: Failed to load sprite from disk, " << IMG_GetError() << "\n";

		m_width = static_cast<const signed short>(m_surface->w);
		m_height = static_cast<const signed short>(m_surface->h);

		m_texture = SDL_CreateTextureFromSurface(rn, m_surface);
	}

	Sprite::Sprite(SDL_Renderer *rn, const SDL_Surface *sf, const float x, const float y, const float w, const float h)
		: m_posx(x), m_posy(y), m_widthResized(w), m_heightResized(h) {
		m_surface = SDL_CreateRGBSurfaceFrom(sf->pixels, sf->w, sf->h, sf->format->BitsPerPixel, sf->pitch,
											 sf->format->Rmask, sf->format->Gmask, sf->format->Bmask,
											 sf->format->Amask);
	
		if (m_surface == nullptr)
			std::cerr << "SRC: sprite.cpp\t ERR: Failed to load sprite from memory, " << SDL_GetError() << "\n";

		m_width = static_cast<const signed short>(m_surface->w);
		m_height = static_cast<const signed short>(m_surface->h);

		m_texture = SDL_CreateTextureFromSurface(rn, m_surface);
	}

	Sprite::~Sprite() {
		SDL_DestroyTexture(m_texture);
		SDL_FreeSurface(m_surface);
	}

	void Sprite::render(SDL_Renderer *renderer) {
		if (m_widthResized == 0)
			m_widthResized = m_width;
		if (m_heightResized == 0)
			m_heightResized = m_height;

		SDL_Rect dst = {
			static_cast<signed int>(m_posx),
			static_cast<signed int>(m_posy),
			static_cast<signed int>(m_widthResized),
			static_cast<signed int>(m_heightResized)
		};

		if (SDL_RenderCopy(renderer, m_texture, NULL, &dst) < 0)
			std::cerr << "SRC: sprite.cpp\t ERR: Failed to render sprite, " << SDL_GetError() << "\n";
	}
};
