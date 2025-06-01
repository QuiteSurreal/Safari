#include "Selector.hpp"
#include "Input.hpp"
#include "ComponentManager.hpp"
#include "../game/Camera.hpp"
#include "../game/Sprite.hpp"

Entity *Selector::static_selected = {};

void Selector::update()
{
	const auto &camera = ComponentManager::getUniqueComponent<Camera>();
	const auto &clicks = Input::getClickList();
	for (const auto &[button, pos] : clicks) {
		auto ray = camera.getRay(pos);
		auto yPos = ray.castToYPlane();
		if (yPos) {
			float distance = std::numeric_limits<float>::max();
			Entity *closest = nullptr;
			ComponentManager::visitComponents<Selector>([yPos, &distance, &closest](const Selector &selector) {
				const float dst = glm::distance(selector.getEntity().getComponent<Transform>().getPosition(), *yPos);
				if (dst < distance) {
					distance = dst;
					closest = &selector.getEntity();
				}
			});
			if (static_selected) {
				static_selected->getComponent<Selector>().selected = false;
				static_selected = nullptr;
			}
			if (closest && distance < closest->getComponent<Selector>().distance) {
				closest->getComponent<Selector>().selected = true;
				static_selected = closest;
			}
		}
	}
}
