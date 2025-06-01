#pragma once

#include "Renderable.hpp"

class Grid : public Renderable::Grid
{
public:

	Grid(Ivec2 size, float scale);
	
	std::optional<Texture> getTexture(Ivec2 tile) const;
	void setTexture(Ivec2 tile, std::optional<Texture> texture);

	std::optional<std::string> getType(Ivec2 tile) const;
	void setType(Ivec2 tile, std::optional<std::string> string);

	std::vector<Ivec2> getAllOfType(std::optional<std::string> type);

	std::optional<Ivec2> getTile(Vec3 position) const;


	float getTileSize() const { return scale; }
	Ivec2 getGridSize() const { return size; }
	Vec3 getPosition() const { return pos; }



private:

	std::unordered_map<Ivec2, std::string> types;

};
