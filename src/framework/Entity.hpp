#pragma once

#include <concepts>
#include <typeindex>
#include <stdexcept>

#include "Component.hpp"
#include "Utils.hpp"

class Entity
{
public:
	
	using Id = Uint;

	const Id id;
	const bool pinned;

	Entity(Id id, bool pinned);

	template <typename T>
	requires std::derived_from<T, Component>
	bool hasComponent() const
	{
		return components.contains(typeid (T));
	}

	template <typename T>
	requires std::derived_from<T, Component>
	T &addComponent();

	template <typename T>
	requires std::derived_from<T, Component>
	T &addComponent(T init);

	template <typename T>
	requires std::derived_from<T, Component>
	bool removeComponent();

	template <typename T>
	requires std::derived_from<T, Component>
	T &getComponent()
	{
		if (!hasComponent<T>()) {
			logFatal("missing component: %s", typeid (T).name());
		}
		return *dynamic_cast<T *>(components.at(typeid (T)));
	}
	template <typename T>
	requires std::derived_from<T, Component>
	const T &getComponent() const
	{
		if (!hasComponent<T>()) {
			logFatal("missing component: %s", typeid (T).name());
		}
		return *dynamic_cast<T *>(components.at(typeid (T)));
	}

	void destroy();

private:

	std::unordered_map<std::type_index, Component *> components;

};

#include "ComponentManager.hpp"

template <typename T>
requires std::derived_from<T, Component>
T &Entity::addComponent()
{
	if (hasComponent<T>()) {
		return getComponent<T>();
	}
	T *component = ComponentManager::addComponent<T>(*this);
	components[typeid (T)] = component;
	return *component;
}

template <typename T>
requires std::derived_from<T, Component>
T &Entity::addComponent(T init)
{
	if (hasComponent<T>()) {
		return getComponent<T>();
	}
	T *component = ComponentManager::addComponent<T>(*this, std::move(init));
	components[typeid (T)] = component;
	return *component;
}

template <typename T>
requires std::derived_from<T, Component>
bool Entity::removeComponent()
{
	if (!hasComponent<T>()) {
		return false;
	}
	ComponentManager::removeComponent<T>(*this);
	components.erase(typeid (T));
	return true;
}
