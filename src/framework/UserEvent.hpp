#pragma once

#include "Component.hpp"
#include "Utils.hpp"

class UserEvent : virtual public Component
{
public:

	virtual void onEvent(const std::string &name, const Entity &target);

	void subscribe(const std::string &name);
	void unsubscribe(const std::string &name);

	static void raiseEvent(const std::string &name, const Entity &entity);
	static void update();

private:

	std::unordered_set<std::string> events;

	static std::unordered_map<std::string, std::unordered_set<UserEvent *>> catalog;
	static std::vector<std::pair<std::string, const Entity *>> buffer;

	void onDetach() override;

};
