#include "Entity.hpp"
#include "ComponentManager.hpp"

std::unordered_map<std::type_index, std::unordered_map<Entity::Id, std::unique_ptr<Component>>> ComponentManager::components;
bool ComponentManager::modified = false;

void ComponentManager::removeAllComponents(const Entity &entity)
{
	for (auto &[type, map] : components) {
		auto iter = map.find(entity.id);
		if (iter != map.end()) {
			assert(iter->second->entity != nullptr);
			iter->second->remove();
			modified = true;
		}
	}
}

void ComponentManager::update()
{
	if (!modified) {
		return;
	}
	const auto updater = [](auto &pair) {
		std::unique_ptr<Component> &component = pair.second;
		if (component->flagAdd && !component->flagRemove) {
			//assert(!component->flagRemove);
			component->onAttach();
			component->flagAdd = false;
			return false;
		}
		if (component->flagRemove) {
			//assert(!component->flagAdd);
			component->onDetach();
			component->flagRemove = false;
			return true;
		}
		return false;
	};
	for (auto &[type, map] : components) {
		std::erase_if(map, updater);
	}
	modified = false;
}