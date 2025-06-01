#include "Utils.hpp"
#include "Texture.hpp"

std::unordered_map<std::string, Uint> Texture::cache;

Texture Texture::fromImage(const std::string &name)
{
	Uint texture {};
	if (cache.contains(name)) {
		texture = cache.at(name);
	}
	else {
		texture = textureFromImage(name);
		cache[name] = texture;
	}
	return texture;
}

Texture Texture::fromBitmapRGBA(Uint w, Uint h, const void *pixels)
{
	Uint texture = textureFromBitmapRGBA(w, h, pixels);
	return texture;
}

bool Texture::operator==(const Texture &other) const
{
	return texture == other.texture;
}