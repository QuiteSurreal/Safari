#include "Grid.hpp"
#include "GridSelector.hpp"

Grid::Grid(Ivec2 size, float scale)
{
	this->size = size;
	this->scale = scale;	
	this->pos = { -size.x / 2.0f, 0.0f, -size.y / 2.0f };
	textures = std::make_unique<std::optional<Texture>[]>(size.x * size.y);
}

std::optional<Texture> Grid::getTexture(Ivec2 tile) const
{
	if (tile.x >= 0 && tile.y >= 0 && tile.x < size.x && tile.y < size.y)
	{
		return at(tile);
	}
	return {};	
}

void Grid::setTexture(Ivec2 tile, std::optional<Texture> texture)
{
	at(tile) = texture;
}

std::vector<Ivec2> Grid::getAllOfType(std::optional<std::string> type)
{
    std::vector<Ivec2> list;

	for(int i = 0; i < size.x; i++){
		for(int j = 0; j < size.y; j++){
			if(getType(Ivec2(i,j)) == type){
				list.push_back(Ivec2(i,j));
			}
		}
	}

	return list;
}

std::optional<Ivec2> Grid::getTile(Vec3 position) const
{
	const Vec3 tile = glm::floor((position - this->pos) / scale);
	if (tile.x < 0 || tile.x >= size.x || tile.z < 0 || tile.z >= size.y) {
		return std::nullopt;
	}
	return Ivec2 {tile.x, tile.z};
}

std::optional<std::string> Grid::getType(Ivec2 tile) const
{
	if (types.contains(tile)) {
		return types.at(tile);
	}
	return {};
}

void Grid::setType(Ivec2 tile, std::optional<std::string> type)
{
	if (type) {
		types[tile] = *type;
	}
	else {
		types.erase(tile);
	}
}
