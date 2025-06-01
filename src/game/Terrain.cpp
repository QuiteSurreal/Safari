#include "Terrain.hpp"

#define DB_PERLIN_IMPL
#include "Perlin.hpp"

Terrain::Terrain(const Grid &grid, Texture texture)
{
	const Ivec2 size = grid.getSize();
	const float scale = grid.getScale();
	const Vec3 pos = grid.getPos();

	std::vector<Vertex> vertices;
	for (int x = -2 * size.x; x < 3 * size.x; ++x) {
		for (int z = -2 * size.y; z < 3 * size.y; ++z) {
			if (x < 0 || x >= size.x || z < 0 || z >= size.y) {
				
				constexpr float scl = 20.0f;
				constexpr float base = 10.0f;
				const Vec3 offset = pos + Vec3{x, 0, z} * scale;
				
				Vec3 tmp;
				float dst;
				
				tmp = offset + Vec3 {0.0f, 0.0f, 1.0f} * scale;
				dst = glm::length(Vec2(tmp.x, tmp.z));
				dst = (std::max(0.0f, dst - 40) / 100.0f);
				tmp.y = (db::perlin(tmp.x / scl, tmp.z / scl) + 1.0f) * base * dst;
				vertices.push_back({tmp, {0.0f, 1.0f}});
				
				tmp = offset + Vec3 {1.0f, 0.0f, 1.0f} * scale;
				dst = glm::length(Vec2(tmp.x, tmp.z));
				dst = (std::max(0.0f, dst - 40) / 100.0f);
				tmp.y = (db::perlin(tmp.x / scl, tmp.z / scl) + 1.0f) * base * dst;
				vertices.push_back({tmp, {1.0f, 1.0f}});
				
				tmp = offset + Vec3 {1.0f, 0.0f, 0.0f} * scale;
				dst = glm::length(Vec2(tmp.x, tmp.z));
				dst = (std::max(0.0f, dst - 40) / 100.0f);
				tmp.y = (db::perlin(tmp.x / scl, tmp.z / scl) + 1.0f) * base * dst;
				vertices.push_back({tmp, {1.0f, 0.0f}});
				
				tmp = offset + Vec3 {0.0f, 0.0f, 1.0f} * scale;
				dst = glm::length(Vec2(tmp.x, tmp.z));
				dst = (std::max(0.0f, dst - 40) / 100.0f);
				tmp.y = (db::perlin(tmp.x / scl, tmp.z / scl) + 1.0f) * base * dst;
				vertices.push_back({tmp, {0.0f, 1.0f}});
				
				tmp = offset + Vec3 {1.0f, 0.0f, 0.0f} * scale;
				dst = glm::length(Vec2(tmp.x, tmp.z));
				dst = (std::max(0.0f, dst - 40) / 100.0f);
				tmp.y = (db::perlin(tmp.x / scl, tmp.z / scl) + 1.0f) * base * dst;
				vertices.push_back({tmp, {1.0f, 0.0f}});
				
				tmp = offset + Vec3 {0.0f, 0.0f, 0.0f} * scale;
				dst = glm::length(Vec2(tmp.x, tmp.z));
				dst = (std::max(0.0f, dst - 40) / 100.0f);
				tmp.y = (db::perlin(tmp.x / scl, tmp.z / scl) + 1.0f) * base * dst;
				vertices.push_back({tmp, {0.0f, 0.0f}});
			}
		}
	}

	this->texture = texture;
	this->mesh = Mesh { vertices };
}
