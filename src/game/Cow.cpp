#include "Cow.hpp"
#include "Transform.hpp"
#include "Labels.hpp"
#include "DestinationBasedMovement/DestinationBasedMovement.hpp"




/*
Transform& trans;
Vec3 Cow::getPos()
{
	const Vec3 &pos = trans.getPosition();
	return pos;
}*/

void Cow::start(Entity &entity)
{
	Transform &trans = entity.getComponent<Transform>();
	trans.relocate({randf(-20, +20), 0.0f, randf(-20, +20)});
	Camera &camera = Labels::getUnique("global").getComponent<Camera>();
	rotate(camera, trans);
}

void Cow::update(Entity &entity)
{
	Camera &camera = Labels::getUnique("global").getComponent<Camera>();
	Transform& trans = entity.getComponent<Transform>();
	rotate(camera, trans);
}

void Cow::rotate(Camera &camera, Transform &trans)
{
	const Vec3 &camPos = camera.getPos();
	const Vec3 &cowPos = trans.getPosition();
	if (glm::distance(camPos, cowPos) > 1e-3f) {
		float angle = std::atan2(camPos.x - cowPos.x, camPos.z - cowPos.z);
		if (getEntity().hasComponent<DestinationBasedMovement>()) { // TODO: sometimes wrong
			const auto& move = getEntity().getComponent<DestinationBasedMovement>();
			const Vec3& dst = move.getDestination();
			const Vec3 vecDst = glm::normalize(dst - trans.getPosition());
			const Vec3 vecCam = glm::normalize(Vec3{ -camera.getForward().z, 0.0f, camera.getForward().x }); // TODO: right
			if (glm::dot(vecDst, vecCam) > 0) {
				angle += PI_F;
			}
		}
		trans.setRotation({ 0.0f, angle, 0.0f });
	}
}