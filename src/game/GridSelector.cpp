#include "GridSelector.hpp"
#include "Camera.hpp"
#include "Grid.hpp"
#include "Labels.hpp"

std::optional<Ivec2> GridSelector::getSelected() const
{
	return selected;
}

const std::vector<Ivec2> GridSelector::getClicks(Input::MouseButton button) const
{
	std::vector<Ivec2> result;

	for (const auto& click : clicks) {
		if (std::get<1>(click) == button) {
			result.push_back(std::get<0>(click));
		}
	}

	return result;
}


void GridSelector::enable()
{
	enabled = true;
	selected = std::nullopt;
	clicks.clear();
}

void GridSelector::disable()
{
	enabled = false;
	selected = std::nullopt;
	clicks.clear();
}

void GridSelector::update(Entity &)
{
	if (enabled) {

		Grid &grid = getEntity().getComponent<Grid>();
		const Camera &camera = Labels::getUnique("global").getComponent<Camera>();

		const Ray ray = camera.getRay(Input::getMousePosition());
		const std::optional<Vec3> pos = ray.castToYPlane();
		selected = pos ? grid.getTile(*pos) : std::nullopt;

		clicks.clear();
		for (auto [button, clickPos] : Input::getClickList()) {
			
			const Ray ray = camera.getRay(clickPos);
			const std::optional<Vec3> pos = ray.castToYPlane();
			const std::optional<Ivec2> tile = pos ? grid.getTile(*pos) : std::nullopt;
			if (tile) {
				std::tuple<Ivec2, Input::MouseButton> tuple = std::tuple(*tile, button);
				clicks.push_back(tuple);
			}
		}
	}
}

