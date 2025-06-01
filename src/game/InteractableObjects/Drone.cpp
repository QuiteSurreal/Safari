#include "Drone.hpp"



Drone::Drone(Vec3 pos)
{
	homePos = pos;
	state = State::Ascending;
	charge = 100.0f;


	doomClock = 0;
	parentStation = nullptr;
	assignedHerd = nullptr;
}


void Drone::start(Entity& entity)
{
	//find a herd, as animals will be wholly reorganized, I think Ill just hardcode this for now
	//assignedHerd = assignHerd();

	//assignedHerdLabel = "zebraHerd:1";
	//assignedHerd = &Labels::getUnique("zebraHerd:1");
	
	assignedHerd = assignHerd();

	entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(homePos.x, 6.0f, homePos.z));
	entity.getComponent<DestinationBasedMovement>().updatePath();
}


void Drone::update(Entity& entity)
{
	Camera& camera = Labels::getUnique("global").getComponent<Camera>();
	Transform& trans = entity.getComponent<Transform>();
	rotate(camera, trans);

	//maybe not every tick, idk how demanding this would be
	checkForSpot();

	if (assignedHerd == nullptr && doomClock < 30)
	{
		assignedHerd = assignHerd();
		doomClock++;
	}

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


	switch (state)
	{
	//go up
	case State::Ascending:
		if (entity.getComponent<DestinationBasedMovement>().getArrived())
		{
			enterPatrolling();
		}
		break;
	//go down to the station
	case State::Descending:
		if (entity.getComponent<DestinationBasedMovement>().getArrived())
		{
			enterCharging();
		}
		break;
	//go to the designated migrate pos of the herd
	case State::Patrolling:
		if (charge > 0.0f)
		{
			charge -= 0.05f;
		}
		else
		{
			enterReturning();
		}
		if (assignedHerd)
		{
			Vec3 dest = assignedHerd->getComponent<DestinationBasedMovement>().getDestination();
			entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(dest.x, 6.0f, dest.z));
			entity.getComponent<DestinationBasedMovement>().updatePath();
			
		}
		break;
	//go back to get charged up
	case State::Returning:
		if (entity.getComponent<DestinationBasedMovement>().getArrived())
		{
			enterDescending();
		}
		break;
	//wait for full charge
	case State::Charging:
		if (charge <= 100.0f)
		{
			charge += 0.4f;
		}
		else
		{
			enterAscending();
		}
		break;

	default:
		//cry
		throw std::runtime_error("Unknown animal(xd) (Drone) state.");
	}


}

void Drone::enterAscending()
{
	entity->getComponent<DestinationBasedMovement>().setDestination(Vec3(homePos.x, 6.0f, homePos.z));
	entity->getComponent<DestinationBasedMovement>().updatePath();
	state = State::Ascending;
}

void Drone::enterDescending()
{
	entity->getComponent<DestinationBasedMovement>().setDestination(Vec3(homePos.x, 0.0f, homePos.z));
	entity->getComponent<DestinationBasedMovement>().updatePath();
	state = State::Descending;
}

void Drone::enterPatrolling()
{
	state = State::Patrolling;
}

void Drone::enterReturning()
{
	entity->getComponent<DestinationBasedMovement>().setDestination(Vec3(homePos.x, 6.0f, homePos.z));
	entity->getComponent<DestinationBasedMovement>().updatePath();
	state = State::Returning;
}

void Drone::enterCharging()
{
	state = State::Charging;
}

//check poachers and decide if they are in spotting range
void Drone::checkForSpot()
{


	const auto& set = Labels::getAll("poacher");
	std::vector<Entity*> an(set.begin(), set.end());


	for (Entity* e : an) 
	{
		float d = distance(
			e->getComponent<Transform>().getPosition(),
			this->entity->getComponent<Transform>().getPosition());

		if (d < 7.81f)
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




void Drone::rotate(Camera& camera, Transform& trans)
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

//herd assignment, a herd can only be assigned to one herd, it will just hover in place if it finds no free herds
Entity* Drone::assignHerd()
{
	std::vector<HerdTypes> names = {
	HerdTypes::Hyena,
	HerdTypes::Lion,
	HerdTypes::Wildebeest,
	HerdTypes::Zebra
	};

	int ran = rand()%4;
	
	HerdTypes type = names[ran];
	const auto& herdMap = Labels::getUnique("eco").getComponent<HerdManager>().getHerdMap();
	auto it = herdMap.find(type);
	if (it != herdMap.end()) 
	{
		const std::vector<Entity*>& herds = it->second;
		for (Entity* herd : herds) {
			if (!herd->getComponent<Labels>().hasLabel("hasDrone"))
			{
				herd->addComponent<Labels>().addLabel("hasDrone");
				return herd;
			}
		}
	}

	return nullptr;
}