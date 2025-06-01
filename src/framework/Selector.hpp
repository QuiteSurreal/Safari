#pragma once

#include "Utils.hpp"
#include "Entity.hpp"
#include "Component.hpp"

#include <optional>

class Selector : virtual public Component
{
public:

	Selector(float distance) : distance {distance} {}

	inline bool isSelected() const { return selected; }
	static inline Entity *getSelected() { return static_selected; }
	static void removeSelected() { static_selected = nullptr; }

	static void update();

private:

	const float distance;
	bool selected = false;
	static Entity *static_selected;

};
