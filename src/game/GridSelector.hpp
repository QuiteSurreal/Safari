#pragma once

#include <optional>

#include "Behaviour.hpp"
#include "tuple"
#include "Input.hpp"

class GridSelector : public Behaviour
{
public:

	void enable();
	void disable();

	std::optional<Ivec2> getSelected() const;
	const std::vector<Ivec2> getClicks(Input::MouseButton button) const;

private:

	bool enabled = false;
	
	void update(Entity &entity) override;

	std::optional<Ivec2> selected;
	std::vector<std::tuple<Ivec2, Input::MouseButton>> clicks;

};
