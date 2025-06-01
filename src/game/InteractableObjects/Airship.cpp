#include "Airship.hpp"


Airship::Airship()
{
	cornerNum = rand()%4;
	state = State::ChangeMode;
	mode = 0;
}


void Airship::start(Entity& entity)
{
 //Seeder::resetWorld();
 //Seeder::seedMenu();
}


void Airship::update(Entity& entity)
{
	Camera& camera = Labels::getUnique("global").getComponent<Camera>();
	Transform& trans = entity.getComponent<Transform>();
	rotate(camera, trans);

	Time::Speed speed = Labels::getUnique("global").getComponent<Time>().getSpeed();
	switch (speed)
	{
	case Time::Speed::Hours:
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.02f);
		break;
	case Time::Speed::Days:
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.04f);
		break;
	case Time::Speed::Weeks:
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.08f);
		break;
	default:
		throw std::runtime_error("Unknown speed state relayed");
		break;
	}

	checkForSpot();

	switch (state)
	{
	//rn doesnt do much, changes the next corner to go to
	case State::ChangeMode:
		if (mode == 0)
		{
			if (cornerNum == 0)
			{
				entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(-20.0f, 6.5f, -20.0f));
				entity.getComponent<DestinationBasedMovement>().updatePath();
			}
			else if (cornerNum == 1)
			{
				entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(-20.0f, 6.5f, 20.0f));
				entity.getComponent<DestinationBasedMovement>().updatePath();
			}
			else if (cornerNum == 2)
			{
				entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(20.0f, 6.5f, 20.0f));
				entity.getComponent<DestinationBasedMovement>().updatePath();
			}
			else if (cornerNum == 3)
			{
				entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(20.0f, 6.5f, -20.0f));
				entity.getComponent<DestinationBasedMovement>().updatePath();
			}
			enterMoving();
		}
		break;

	//go to the assigned corner, then determine the next
	case State::Moving:
		if (entity.getComponent<DestinationBasedMovement>().getArrived())
		{
			cornerNum = (cornerNum + 1) % 4;
			enterChangeMode();
		}
		break;

	default:
		throw std::runtime_error("Unknown animal(xd) (Airship) state.");
	}
}


void Airship::enterMoving()
{
	state = State::Moving;
}

void Airship::enterChangeMode()
{
	state = State::ChangeMode;
}


void Airship::checkForSpot()
{


	const auto& set = Labels::getAll("poacher");
	std::vector<Entity*> an(set.begin(), set.end());


	for (Entity* e : an)
	{
		float d = distance(
			e->getComponent<Transform>().getPosition(),
			this->entity->getComponent<Transform>().getPosition());

		if (d < 11.0f)
		{
			e->getComponent<Poacher>().setSpotted(true);
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


void Airship::rotate(Camera& camera, Transform& trans)
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