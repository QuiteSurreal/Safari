#pragma once

#include <unordered_set>
#include <unordered_map>

#include "Utils.hpp"
#include "Component.hpp"

class Labels : virtual public Component
{
public:
	
	void onDetach() override;

	bool addLabel(const std::string &label);
	bool removeLabel(const std::string &label);
	bool hasLabel(const std::string &label);
	const std::unordered_set<std::string> &getLabels();

	static const std::unordered_set<Entity *> &getAll(const std::string &label);
	static Entity &getUnique(const std::string &label);
	static bool isLabelUsed(const std::string &label);

private:

	std::unordered_set<std::string> labels;

	static std::unordered_map<std::string, std::unordered_set<Entity *>> catalog;

};
