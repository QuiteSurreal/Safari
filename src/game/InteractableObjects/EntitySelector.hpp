#pragma once

#include <optional>

#include "Behaviour.hpp"
#include "tuple"
#include "Input.hpp"
#include "../UIBehaviour.hpp"
#include "Labels.hpp"
#include "../Camera.hpp"
#include "../Factory.hpp"
#include "HunterHut.hpp"
#include "Selector.hpp"
#include "../Animals/Zebra.hpp"
#include "../GameManager.hpp"

class EntitySelector : public Behaviour
{
public:

	void enable();
	void disable();

	Entity* getSelected() const;
	const std::vector<Vec3> getClicks(Input::MouseButton button) const;


	EntitySelector()
		: uib(Labels::getUnique("global").getComponent<UIBehaviour>())
	{
	}

private:

	UIBehaviour& uib;

	bool enabled = false;

	void update(Entity& entity) override;
	Entity* getOpenHut();

	Entity* selected;
	Entity* target;

	std::vector<std::tuple<Vec3, Input::MouseButton>> clicks;

	bool hunterDirectMode = false;

};
