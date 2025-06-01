#include "Entity.hpp"
#include "Core.hpp"

Entity::Entity(Id id, bool pinned) : id {id}, pinned {pinned} {}

void Entity::destroy()
{
	Core::destroyEntity(*this);
}