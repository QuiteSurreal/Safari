#include <algorithm>

#include "UserEvent.hpp"

std::unordered_map<std::string, std::unordered_set<UserEvent *>> UserEvent::catalog;
std::vector<std::pair<std::string, const Entity *>> UserEvent::buffer;

void UserEvent::onEvent(const std::string &name, const Entity &target)
{
	(void) name;
	(void) target;
}

void UserEvent::subscribe(const std::string &name)
{
	if (events.contains(name)) {
		return;
	}
	catalog[name].insert(this);
	events.insert(name);
}

void UserEvent::unsubscribe(const std::string &name)
{
	if (!events.contains(name)) {
		return;
	}
	catalog[name].erase(this);
	events.erase(name);
}

void UserEvent::raiseEvent(const std::string &name, const Entity &entity)
{
	buffer.push_back({name, &entity});
}

void UserEvent::update()
{
	for (const auto &[name, target] : buffer) {
		for (UserEvent *listener : catalog[name]) {
			listener->onEvent(name, *target);
		}
	}
	buffer.clear();
}

void UserEvent::onDetach()
{
	for (const std::string &name : std::unordered_set<std::string>{events}) {
		unsubscribe(name);
	}
	const Entity *entity = &getEntity();
	std::erase_if(buffer, [entity](const auto &entry) {
		return entry.second == entity;
	});
}
