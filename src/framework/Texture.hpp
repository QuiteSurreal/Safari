#pragma once

#include "Utils.hpp"

class Texture
{
public:

	inline Uint getTexture() const { return texture; }

	Texture() {}
	Texture(const Texture &other) : texture { other.texture } {}
	Texture &operator=(const Texture &other) { this->texture = other.texture; return *this; }

	void destroy();

	bool operator==(const Texture &other) const;

	static Texture fromImage(const std::string &name);
	static Texture fromBitmapRGBA(Uint w, Uint h, const void *pixels);

private:

	Uint texture;

	Texture(Uint texture) : texture {texture} {}

	static std::unordered_map<std::string, Uint> cache;

	static Uint textureFromImage(const std::string &name);
	static Uint textureFromBitmapRGBA(Uint w, Uint h, const void *pixels);
};

namespace std
{
	template <>
	struct hash<Texture>
	{
		size_t operator()(const Texture &texture) const
		{
			return texture.getTexture();
		}
	};
}
