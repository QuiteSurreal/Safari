#pragma once

#include <unordered_map>
#include <unordered_set>

#include "Entity.hpp"

class Core
{
public:

	static void initialize();
	static void loop();

	static Entity &createEntity(bool pinned = false);
	static void destroyEntity(const Entity &entity);
	static void destroyAllEntities();

private:

	static void clean();
	static void swapFrame();

	static Entity::Id ids;
	static std::unordered_map<Entity::Id, std::unique_ptr<Entity>> entities;
	static std::unordered_set<Entity::Id> entitiesToRemove;

};
