#include "Hunter.hpp"

Hunter::Hunter(const Vec3 startingPos)
{
    Hunter::hunterLabel = "";
    Hunter::startingPos = startingPos;
    Hunter::patrolPos = startingPos;
    Hunter::localId = 0;
    Hunter::hutId = 0;
    Hunter::state = State::Idling;
    Hunter::target = nullptr;
    Hunter::idleCounter = 0;
    Hunter::firerate = 0;
}


void Hunter::start(Entity& entity)
{
   
}

void Hunter::update(Entity& entity)
{
    Camera& camera = Labels::getUnique("global").getComponent<Camera>();
	Transform& trans = entity.getComponent<Transform>();
	rotate(camera, trans);

    entity.getComponent<DestinationBasedMovement>().setSpeed(Labels::getUnique("global").getComponent<GameManager>().getSpeed() * 0.03f);


    //laze around the base until a poacher is spotted or an animal is set to be killed
    //this could be done some ways:
    //the hunter can be selected, then you can click someplace
    //that could be: ground-> the hunter goes there and if there are spotted poachers in range it starts auto shooting at them
    //animal directly-> the hunter targets the entity directly and tries to hunt it down

    if (idleCounter > 0 || firerate > 0)
    {
        idleCounter -= 1;
        firerate -= 1;
    }
    float d = 0;
    switch(state)
    {
        case State::Idling:
            target = checkForSpotted();
            if (target && target->getComponent<Targetable>().canTarget() && target->getComponent<Health>().getAlive())
            {
                enterShooting();
            }
            else if (entity.getComponent<DestinationBasedMovement>().getArrived() && idleCounter <= 0)
            {
                idleCounter = 100;
                enterIdling();
            }
            break;

        case State::Travelling:
            target = checkForSpotted();
            if (target && target->getComponent<Targetable>().canTarget() && target->getComponent<Health>().getAlive())
            {
                enterShooting();
            }
            else if (entity.getComponent<DestinationBasedMovement>().getArrived())
            {
                enterIdling();
            }
            break;

        case State::Hunting:

            if (target && target->getComponent<Targetable>().canTarget() && target->getComponent<Health>().getAlive())
            {
                d = distance(
                    target->getComponent<Transform>().getPosition(),
                    entity.getComponent<Transform>().getPosition());
                if (d <= 10)
                {
                    enterShooting();
                }
                else
                {
                    entity.getComponent<DestinationBasedMovement>().setDestination(target->getComponent<DestinationBasedMovement>().getDestination());
                    entity.getComponent<DestinationBasedMovement>().updatePath();
                }
            }
            else
            {
                idleCounter = 100;
                enterIdling();
            }
            break;

        case State::Shooting:
            if (target && target->getComponent<Targetable>().canTarget() && target->getComponent<Health>().getAlive())
            {
                d = distance(
                    target->getComponent<Transform>().getPosition(),
                    entity.getComponent<Transform>().getPosition());
                if (d <= 15.0f && firerate <= 0)
                {
                    target->getComponent<Health>().decreaseHealth(1);
                    if (!target->getComponent<Health>().getAlive())
                    {
                        idleCounter = 300;
                        target->getComponent<Targetable>().setTargetable(false);
                        Labels::getUnique("global").getComponent<GameManager>().addMoney(500);
                        enterIdling();
                    }
                    else
                    {
                        firerate = 80;
                    }
                }
            }
            else
            {
                enterIdling();
            }

            break;

        default:
            break;
    }


}

void Hunter::enterIdling()
{
    if (target)
    {
        target->getComponent<Targetable>().removeTargeting();
        target = nullptr;
    }
    state = State::Idling;
    float randX = randf(-2.5f, 2.5f);
    float randZ = randf(-2.5f, 2.5f);
    entity->getComponent<DestinationBasedMovement>().setDestination(Vec3(randX + patrolPos.x, 0.0f, randZ + patrolPos.z));
    entity->getComponent<DestinationBasedMovement>().updatePath();
}

void Hunter::enterTravelling(Vec3 point)
{
    if (target)
    {
        target->getComponent<Targetable>().removeTargeting();
        target = nullptr;
    }
    state = State::Travelling;
    patrolPos = point;
    entity->getComponent<DestinationBasedMovement>().setDestination(patrolPos);
    entity->getComponent<DestinationBasedMovement>().updatePath();
}

void Hunter::enterHunting(Entity* t)
{
    if (target)
    {
        target->getComponent<Targetable>().removeTargeting();
        target = nullptr;
    }
    state = State::Hunting;
    target = t;
    target->getComponent<Targetable>().target();

    //target->getComponent<Zebra>()
    entity->getComponent<DestinationBasedMovement>().setDestination(target->getComponent<DestinationBasedMovement>().getDestination());
    entity->getComponent<DestinationBasedMovement>().updatePath();
}

void Hunter::enterShooting()
{
    state = State::Shooting;
}




//essentially get the closest spotted poacher
Entity* Hunter::checkForSpotted()
{
    const auto& set = Labels::getAll("poacher");
	std::vector<Entity*> an(set.begin(), set.end());

	Entity* closest = nullptr;
	float minDist = std::numeric_limits<float>::max();


	for (Entity* e : an) {
		float d = distance(
			e->getComponent<Transform>().getPosition(),
			this->entity->getComponent<Transform>().getPosition());

		if (d < minDist && e->getComponent<Poacher>().getSpotted() && e->getComponent<Health>().getAlive()) {
			minDist = d;
			closest = e;
		}
	}

    if (closest && minDist < 6.0f && closest->getComponent<Targetable>().canTarget())
    {
        closest->getComponent<Targetable>().target();
        return closest;
    }

    return nullptr;
}

void Hunter::rotate(Camera& camera, Transform& trans)
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