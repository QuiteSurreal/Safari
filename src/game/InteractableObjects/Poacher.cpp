#include "Poacher.hpp"


Poacher::Poacher(const Vec3& startPos) : startingPos(startPos)
{
	Poacher::localId = 1;
	Poacher::poacherLabel = "";
	Poacher::carcassName = "";
	Poacher::preyName = "";

	Poacher::state = State::Scouting;

	Poacher::idleCounter = 500;

	Poacher::prey = nullptr;
	Poacher::trophy = nullptr;

	Poacher::spotted = false;
}

void Poacher::start(Entity& entity)
{
	Poacher::carcassName = "carcass:" + poacherLabel;
}

void Poacher::update(Entity& entity)
{
	//this is how everything should go into a death spiral, stuff that doesnt have health, just remove that part (mayb an integral bool, or timer like carcass)
	if (!entity.getComponent<Health>().getAlive())
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
			return;
		}
	}

	Camera& camera = Labels::getUnique("global").getComponent<Camera>();
	Transform& trans = entity.getComponent<Transform>();
	rotate(camera, trans);

	entity.getComponent<DestinationBasedMovement>().setSpeed(Labels::getUnique("global").getComponent<GameManager>().getSpeed() * 0.02f); 

	//unless not spotted, appear
	if (spotted)
	{
		entity.getComponent<Sprite>().setOpacity(0.0f);
	}
	else
	{
		entity.getComponent<Sprite>().setOpacity(100.0f);
	}

	//if there is a label with the name of the trophy, go and get it
	if (Labels::isLabelUsed(carcassName) && trophy == nullptr)
	{
		Entity& carcass = Labels::getUnique(carcassName);
		setTrophy(carcass);
		carcass.getComponent<Targetable>().target();
		entity.getComponent<DestinationBasedMovement>().setDestination(trophy->getComponent<Transform>().getPosition());
		entity.getComponent<DestinationBasedMovement>().updatePath();
		enterGettingTrophy();
	}

	//if the prey is dead, remove it from targeting and drop the pointer
	if (prey && !prey->getComponent<Health>().getAlive())
	{
		prey->getComponent<Targetable>().removeTargeting();
		prey = nullptr;
	}

	float d = 0;
	switch (state)
	{
	case State::Scouting:
		//wait a bit, then chose an animal
		if (idleCounter > 0) {
			int r = rand() % 3;
			idleCounter -= r;
		}
		else
		{
			Poacher::prey = getClosestAnimal();
			if (prey && prey->getComponent<Targetable>().canTarget())
			{
				enterHunting();
			}
			else
			{
				enterLeaving();
			}
		}
		break;

	case State::Hunting:
		//get close enough for shooting
		if (prey && prey->getComponent<Health>().getAlive() && prey->getComponent<Targetable>().canTarget())
		{
			d = distance(
				prey->getComponent<Transform>().getPosition(),
				entity.getComponent<Transform>().getPosition());
			if (d <= 10)
			{
				entity.getComponent<DestinationBasedMovement>().setDestination(entity.getComponent<Transform>().getPosition());
				entity.getComponent<DestinationBasedMovement>().updatePath();
				idleCounter = 100;
				enterShooting();
			}
			else if (prey->getComponent<Targetable>().canTarget())
			{
				entity.getComponent<DestinationBasedMovement>().setDestination(prey->getComponent<Transform>().getPosition());
				entity.getComponent<DestinationBasedMovement>().updatePath();
			}
		}
		else
		{
			enterLeaving();
		}

		break;

	case State::Shooting:
		if (idleCounter > 0) {
			idleCounter -= 1;
		}
		else if (prey && prey->getComponent<Health>().getAlive() && prey->getComponent<Targetable>().canTarget())
		{
			d = distance(
				prey->getComponent<Transform>().getPosition(),
				entity.getComponent<Transform>().getPosition());
			if (d <= 15 && prey->getComponent<Targetable>().canTarget())
			{
				//shoot
				prey->getComponent<Health>().decreaseHealth(1, poacherLabel);
				idleCounter = 100;
			}
			else if (prey->getComponent<Targetable>().canTarget())
			{
				//if the animal gets out of range mid shooting
				entity.getComponent<DestinationBasedMovement>().setDestination(prey->getComponent<Transform>().getPosition());
				entity.getComponent<DestinationBasedMovement>().updatePath();
				enterHunting();
			}
		}
		else
		{
			//if the animal dies, enter leaving, bit of a failsafe
			enterLeaving();
		}
		break;
	case State::GettingTrophy:
		//pick up dead animal
		if (entity.getComponent<DestinationBasedMovement>().getArrived())
		{
			if (trophy->getComponent<Targetable>().canTarget())
			{
				trophy->getComponent<Carcass>().getDragged(startingPos, entity.getComponent<DestinationBasedMovement>().getSpeed());
			}
			enterLeaving();
		}
		break;
	case State::Leaving:
		//return to starting pos, with dead animal
		//you can only get a trophy pointer here, if its certain that it exists/still exists, so this shouldnt be a problem
		if (trophy)
		{
			if (entity.getComponent<DestinationBasedMovement>().getArrived())
			{
				entity.destroy();
				trophy->getComponent<Carcass>().setDeathSpiral(true);
				trophy->getComponent<Targetable>().setTargetable(false);
				trophy->getComponent<Targetable>().removeTargeting();
			}
			else
			{
				trophy->getComponent<Carcass>().getDragged(startingPos, entity.getComponent<DestinationBasedMovement>().getSpeed());
			}
		}
		else if (entity.getComponent<DestinationBasedMovement>().getArrived())
		{
			entity.destroy();
		}
		break;

	default:
		throw std::runtime_error("Unknown animal(lol) (Poacher) state.");
		break;
	}
}

void Poacher::setUniqueId(int id)
{
	poacherLabel = "poacher:" + std::to_string(id);
	entity->addComponent<Labels>().addLabel(poacherLabel);
}

void Poacher::enterLeaving()
{
	spotted = true;
	prey = nullptr;
	entity->getComponent<DestinationBasedMovement>().setDestination(startingPos);
	entity->getComponent<DestinationBasedMovement>().updatePath();
	state = State::Leaving;
}


void Poacher::enterGettingTrophy()
{
	spotted = true;
	state = State::GettingTrophy;
}


void Poacher::enterShooting()
{
	spotted = true;
	state = State::Shooting;
}

void Poacher::enterHunting()
{
	spotted = false;
	state = State::Hunting;
}

void Poacher::enterScouting()
{
	spotted = false;
	state = State::Scouting;
}

void Poacher::setTrophy(Entity& newTrophy)
{
	trophy = &newTrophy;
}

//get the closest animal from spawn and start hunting it
//the poacher is dedicated to this animal, if it dies, before he himself kills it, he just leaves
Entity* Poacher::getClosestAnimal()
{

	
	const auto& set = Labels::getAll("animal");
	std::vector<Entity*> an(set.begin(), set.end());



	Entity* closest = nullptr;
	float minDist = std::numeric_limits<float>::max();


	for (Entity* e : an) {
		float d = distance(
			e->getComponent<Transform>().getPosition(),
			this->entity->getComponent<Transform>().getPosition());

		if (d < minDist && e->getComponent<Health>().getAlive() && e->getComponent<Targetable>().canTarget()) {
			minDist = d;
			closest = e;
		}
	}

	//std::shared_ptr<Entity> closest = std::make_shared<Entity>();
	if (closest)
	{
		closest->getComponent<Targetable>().target();
	}
	

	return closest;
}

//destroy self, and if was dragging a corpse, make it stop
void Poacher::die()
{
	if (trophy)
	{
		trophy->getComponent<DestinationBasedMovement>().setDestination(trophy->getComponent<Transform>().getPosition());
		trophy->getComponent<DestinationBasedMovement>().updatePath();
		trophy->getComponent<Targetable>().removeTargeting();
	}

	Labels::getUnique("global").getComponent<GameManager>().addMoney(1000);
	entity->destroy();
}

void Poacher::rotate(Camera& camera, Transform& trans)
{
	const Vec3& camPos = camera.getPos();
	const Vec3& cowPos = trans.getPosition();
	if (glm::distance(camPos, cowPos) > 1e-3f) {
		float angle = std::atan2(camPos.x - cowPos.x, camPos.z - cowPos.z);
		if (getEntity().hasComponent<DestinationBasedMovement>()) { // TODO: sometimes wrong
			const auto& move = getEntity().getComponent<DestinationBasedMovement>(); //TODO: remove TODO
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