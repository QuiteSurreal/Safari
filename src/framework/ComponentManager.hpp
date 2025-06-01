#pragma once

#include <unordered_map>
#include <typeindex>
#include <stdexcept>
#include <concepts>
#include <algorithm>

#include "Utils.hpp"
#include "Entity.hpp"
#include "Component.hpp"

class ComponentManager
{
public:

	template <typename T>
	requires std::derived_from<T, Component>
	static T *addComponent(Entity &entity)
	{
		auto derived = std::make_unique<T>();
		derived->add(entity);
		T *result = derived.get();
		std::unique_ptr<Component> base = std::move(derived);
		components[typeid (T)].emplace(entity.id, std::move(base));
		modified = true;
		return result;
	}

	template <typename T>
	requires std::derived_from<T, Component>
	static T *addComponent(Entity &entity, T init)
	{
		auto derived = std::make_unique<T>(std::move(init));
		derived->add(entity);
		T *result = derived.get();
		std::unique_ptr<Component> base = std::move(derived);
		components[typeid (T)].emplace(entity.id, std::move(base));
		modified = true;
		return result;
	}

	template <typename T>
	requires std::derived_from<T, Component>
	static void removeComponent(Entity &entity)
	{
		auto &component = components.at(typeid (T)).at(entity.id);
		component->remove();
		modified = true;
	}

	template <typename T>
	requires std::derived_from<T, Component>
	static T &getUniqueComponent()
	{
		T *result = nullptr;
		visitComponents<T>([&result](T &component) {
			if (result) {
				logFatal("there are multiple components of: %s", typeid (T).name());
			}
			result = &component;
		});
		if (!result) {
			logFatal("there is no component of: %s", typeid (T).name());
		}
		return *result;
	}

	template <typename T>
	requires std::derived_from<T, Component>
	static void visitComponents(auto visitor)
	{
		for (auto &[type, map] : components) {
			if (!map.empty() && dynamic_cast<T*>(map.begin()->second.get())) {
				for (auto& [entity, component] : map) {
					if (!component->flagAdd && !component->flagRemove) {
						assert(component->entity);
						visitor(*dynamic_cast<T*>(component.get()));
					}
				}
			}
		}
	}

	static void removeAllComponents(const Entity &entity);
	
	static void update();

private:

	static std::unordered_map<std::type_index, std::unordered_map<Entity::Id, std::unique_ptr<Component>>> components;
	static bool modified;

};
