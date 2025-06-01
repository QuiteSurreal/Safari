#include "Labels.hpp"

std::unordered_map<std::string, std::unordered_set<Entity *>> Labels::catalog;

void Labels::onDetach()
{
	for (const std::string &label : labels) {
		catalog[label].erase(&getEntity());
	}
	labels.clear();
}

bool Labels::addLabel(const std::string &label)
{
	if (hasLabel(label)) {
		return false;
	}
	catalog[label].insert(&getEntity());
	labels.insert(label);
	return true;
}

bool Labels::removeLabel(const std::string &label)
{
	if (!hasLabel(label)) {
		return false;
	}
	catalog[label].erase(&getEntity());
	labels.erase(label);
	return true;
}

bool Labels::hasLabel(const std::string &label)
{
	return labels.contains(label);
}

const std::unordered_set<std::string> &Labels::getLabels()
{
	return labels;
}

const std::unordered_set<Entity *> &Labels::getAll(const std::string &label)
{
	return catalog[label];
}

Entity &Labels::getUnique(const std::string &label)
{
	const auto &all = getAll(label);
    if (all.size() < 1) {
        logFatal("label does not exists. Label: %s", label.c_str());
    }
    else if(all.size() > 1){
        logFatal("label is not unique (multiple): %s", label.c_str());
    }
    return *(*all.begin());
}

bool Labels::isLabelUsed(const std::string &label)
{
	return catalog.contains(label);
}
