#pragma once

#include "Grid.hpp"
#include "Renderable.hpp"

class Terrain : public Renderable::Terrain
{
public:
	Terrain(const Grid &grid, Texture texture);
};
