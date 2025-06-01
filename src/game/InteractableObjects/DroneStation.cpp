#include "DroneStation.hpp"



DroneStation::DroneStation(Vec3 position)
{
	pos = position;
}

void DroneStation::start(Entity& entity)
{
	//create drone here
	Factory::createDrone(pos, &entity);
}

void DroneStation::update(Entity& entity)
{
	//tbh it doesnt really do anything
	Camera& camera = Labels::getUnique("global").getComponent<Camera>();
	Transform& trans = entity.getComponent<Transform>();
	rotate(camera, trans);
}

void DroneStation::setUniqueId(int id)
{
	stationLabel = "droneStation:" + std::to_string(id);
	entity->addComponent<Labels>().addLabel(stationLabel);
}

void DroneStation::rotate(Camera& camera, Transform& trans)
{
	const Vec3& camPos = camera.getPos();
	const Vec3& cowPos = trans.getPosition();
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