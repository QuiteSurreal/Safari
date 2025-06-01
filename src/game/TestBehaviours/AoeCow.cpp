#include "AoECow.hpp"

void AoECow::update(Entity& entity) {

	const Camera& cam = Labels::getUnique("global").getComponent<Camera>();
	const Ray ray = cam.getRay(Input::getMousePosition());
	const std::optional<Vec3> destPos = ray.castToYPlane();

	if (destPos.has_value() && Input::isMouseButtonPressed(Input::MouseButton::Left)) {
		entity.getComponent<DestinationBasedMovement>().setDestination(*destPos);
		entity.getComponent<DestinationBasedMovement>().updatePath();
	}

}
