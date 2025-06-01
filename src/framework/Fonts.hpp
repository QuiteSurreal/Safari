#pragma once

#include "Texture.hpp"

class Fonts
{
public:

	static void init();
	static void destroy();

	static Texture createTextTexture(const char *text, int fontSize, Vec4 color, Ivec2 &size);

private:

};
