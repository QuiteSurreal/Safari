#pragma once

#include "Utils.hpp"

struct Vertex
{
	Vec3 pos;
	Vec2 uv;
};

class Mesh
{
public:

	Mesh() = default;
	Mesh(const std::vector<Vertex> &vertices);
	void destroy();

	void render() const;

private:

	Uint VAO, VBO;
	Uint count;

};
