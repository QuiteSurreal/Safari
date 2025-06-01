#include "Component.hpp"
#include "Utils.hpp"

Entity &Component::getEntity() const
{
	assert(entity);
	return *entity;
}

void Component::add(Entity &entity)
{
	this->entity = &entity;
	flagAdd = true;
}

void Component::remove()
{
	flagRemove = true;
}

void Component::onAttach()
{
}

void Component::onDetach()
{
}
