#include "Carcass.hpp"



Carcass::Carcass()
{
	Carcass::age = 0;
	Carcass::lastHour = Labels::getUnique("global").getComponent<Time>().getHours();
	Carcass::remainingCalories = 0;
	Carcass::deathSpiral = false;
}

void Carcass::start(Entity& entity)
{

}

void Carcass::update(Entity& entity)
{
	//if set true, wait until all targetings are removed, and thenn destroy self
	if (deathSpiral)
	{
		if (entity.getComponent<Targetable>().getTargetingCount() > 0)
		{
			entity.getComponent<Targetable>().setTargetable(false);
			//entity.getComponent<Sprite>().setOpacity(0.0f);
			return;
		}
		else
		{
			die();
		}
	}


	/*Uint currHour = Labels::getUnique("global").getComponent<Time>().getHours();
	if (currHour != lastHour)
	{
		rot();
		lastHour = currHour;
	}*/
	Camera& camera = Labels::getUnique("global").getComponent<Camera>();
	Transform& trans = entity.getComponent<Transform>();
	//rotate(camera, trans);
}


/*void Carcass::rot()
{
	age++;
	if (age > 24 && entity->getComponent<Labels>().hasLabel("Spooky"))
	{
		entity->getComponent<Labels>().removeLabel("Spooky");
	}
	if (age > 120)
	{
		deathSpiral = true;
	}
}*/

void Carcass::die()
{
	entity->destroy();
}

//gets a destination and speed, that should match the entity dragging it
void Carcass::getDragged(Vec3 dest, float speed)
{
	entity->getComponent<DestinationBasedMovement>().setDestination(dest);
	entity->getComponent<DestinationBasedMovement>().setSpeed(speed);
	entity->getComponent<DestinationBasedMovement>().updatePath();
}

void Carcass::rotate(Camera& camera, Transform& trans)
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