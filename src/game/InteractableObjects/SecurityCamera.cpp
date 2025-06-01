#include "SecurityCamera.hpp"

SecurityCamera::SecurityCamera()
{

}

void SecurityCamera::start(Entity& entity)
{

}

void SecurityCamera::update(Entity& entity)
{
    Camera& camera = Labels::getUnique("global").getComponent<Camera>();
	Transform& trans = entity.getComponent<Transform>();
	rotate(camera, trans);

	checkForSpot();
}


//check poachers and decide if they are in spotting range
void SecurityCamera::checkForSpot()
{
	const auto& set = Labels::getAll("poacher");
	std::vector<Entity*> an(set.begin(), set.end());


	for (Entity* e : an)
	{
		float d = distance(
			e->getComponent<Transform>().getPosition(),
			this->entity->getComponent<Transform>().getPosition());

		if (d < 7.0f)
		{
			e->getComponent<Poacher>().setSpotted(true);
			//e->getComponent<Poacher>().setAlreadySeen(true);
		}
		else if (e->getComponent<Poacher>().getState() != Poacher::State::Shooting &&
			e->getComponent<Poacher>().getState() != Poacher::State::GettingTrophy &&
			e->getComponent<Poacher>().getState() != Poacher::State::Leaving &&
			!e->getComponent<Poacher>().getSpotted()) 
		{
			e->getComponent<Poacher>().setSpotted(false);
		}
	}
}


void SecurityCamera::rotate(Camera& camera, Transform& trans)
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