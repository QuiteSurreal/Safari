#ifndef HEADLESS
#include <SDL2/SDL_ttf.h>
#endif // HEADLESS

#include "Fonts.hpp"
#include "Utils.hpp"

void Fonts::init()
{
#ifndef HEADLESS
	assert(TTF_Init() != -1);
#endif // HEADLESS
}

void Fonts::destroy()
{
#ifndef HEADLESS
	TTF_Quit();
#endif // HEADLESS
}

Texture Fonts::createTextTexture(const char *text, int fontSize, Vec4 color, Ivec2 &size)
{
#ifndef HEADLESS
	const std::string fontPath = DIR_FONTS + std::string{"font.otf"};
	TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize);
	assert(font);
	
	const SDL_Color colorSDL = {
		(Uint8) (color.r * 0xFF),
		(Uint8) (color.g * 0xFF),
		(Uint8) (color.b * 0xFF),
		(Uint8) (color.a * 0xFF)
	};
	SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, colorSDL);
	assert(surface);
	if (surface->format->format != SDL_PIXELFORMAT_RGBA32) {
		SDL_Surface *surfaceRGBA = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
		assert(surfaceRGBA);
		SDL_FreeSurface(surface);
		surface = surfaceRGBA;
	}
	assert(surface->format->format == SDL_PIXELFORMAT_RGBA32);

	size.x = surface->w;
	size.y = surface->h;
	const Texture texture = Texture::fromBitmapRGBA(surface->w, surface->h, surface->pixels);

	SDL_FreeSurface(surface);	
	TTF_CloseFont(font);

	return texture;
#else
	return {};
#endif // HEADLESS
}
