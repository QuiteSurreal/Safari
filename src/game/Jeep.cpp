#include "Jeep.hpp"
#include <algorithm>
#include "../game/DestinationBasedMovement/DestinationBasedMovement.hpp"


void Jeep::start(Entity& self) {
    currentRoadIndex = 0;
    roadList = road.GetRoadList();

    if(roadList.size() == 0){
        logDebug("road list empty!");
        
        roadList.push_back(Ivec2(pos.x + 25, pos.z + 25));
        roadList.push_back(Ivec2(pos.x + 25, pos.z + 1 + 25));
        roadList.push_back(Ivec2(pos.x + 25, pos.z + 2 + 25));
        roadList.push_back(Ivec2(pos.x + 25, pos.z + 3 + 25));
        roadList.push_back(Ivec2(pos.x + 25, pos.z + 4 + 25));
        
    }

    std::unordered_set<std::string> seenAnimals;
    std::unordered_set<std::string> seenTypes;

    //Transform& trans = self.getComponent<Transform>();
    //pos = trans.getPosition();
    //pos.x = pos.x + 25;
    //pos.z = pos.z + 25;
    //AtLocation


    //pos = roadList[0];
}


void Jeep::update(Entity &entity)
{
	Transform& trans = entity.getComponent<Transform>();
	rotate(camera, trans);
    //AtLocation(roadList[currentRoadIndex], entity);

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

    checkArrivedTimer += 0.1f * Labels::getUnique("global").getComponent<GameManager>().getSpeed();
    //logDebug(std::to_string(checkArrivedTimer).c_str());
    pos = trans.getPosition();
    pos.x = pos.x + 25;
    pos.z = pos.z + 25;
    //logDebug(std::to_string(pos.x).c_str());

    if(checkArrivedTimer >= checkArrivedInterval){
        checkArrivedTimer = 0.f;
        //logDebug(std::to_string(currentRoadIndex).c_str());
        AtLocation(roadList[currentRoadIndex], entity);
    }

    checkForAnimal();
}

void Jeep::rotate(Camera &camera, Transform &trans)
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

void Jeep::Move(){

}

void Jeep::checkForAnimal()
{
    const auto& set = Labels::getAll("animal");
	std::vector<Entity*> an(set.begin(), set.end());


	for (Entity* e : an) 
	{
		float d = distance(
			e->getComponent<Transform>().getPosition(),
			this->entity->getComponent<Transform>().getPosition());

        std::string animalLabel;
        std::string type;
		if (d < 7.81f)
		{
            if (e->hasComponent<HerbivoreAI>())
		    {
		    	animalLabel = e->getComponent<HerbivoreAI>().getAnimalLabel();
                if (animalLabel.find("zebra") != std::string::npos) { type = "zebra"; } else { type = "wildebeest"; }

		    }
		    else
		    {
		    	animalLabel = e->getComponent<HerbivoreAI>().getAnimalLabel();
                if (animalLabel.find("lion") != std::string::npos) { type = "lion"; } else { type = "hyena"; }
		    }
            if (seenAnimals.find(animalLabel) == seenAnimals.end()) 
            {
                // Animal not seen yet
                seenAnimals.insert(animalLabel);
                score += 0.01f;
                if (seenTypes.find(type) == seenTypes.end())
                {
                    seenTypes.insert(type);
                    score += 1.0f;
                }
            }
		}
	}
}

void Jeep::setUniqueId(int id)
{
	jeepLabel = "Jeep: " + std::to_string(id);
	entity->addComponent<Labels>().addLabel(jeepLabel);
}

void Jeep::AtLocation(Ivec2 loc, Entity &ent){
    Ivec2 posI = Ivec2(std::floor(pos.x), std::floor(pos.z));
    
    //Check if we are at the next checkpoint
    if( posI.x == loc.x && posI.y == loc.y){

        //If we are moving back and arrive to the entry ponint
        if((state == travelingBack || state == waitingToScore) && currentRoadIndex == 0){
            #ifndef HEADLESS
            global->getComponent<GameManager>().addJeep();
            global->getComponent<GameManager>().removeEnRoute();
            #endif
            entity->destroy();
        }

        //If we arrived, we need to fo back
        if(currentRoadIndex == roadList.size() - 1 && state == travelingForward){
            state = waitingToScore;
            #ifndef HEADLESS
            Score();
            #endif
        }

        //If we have more checkpoint, go to the next
        if(state == travelingForward){
            if(currentRoadIndex < roadList.size() - 1){
                //logDebug("increasing curreontRoadIndex");
                currentRoadIndex++;
                ent.getComponent<DestinationBasedMovement>().setDestination(Vec3(roadList[currentRoadIndex].x - 25 + 0.5, pos.y, roadList[currentRoadIndex].y -25 + 0.5));
                ent.getComponent<DestinationBasedMovement>().updatePath();
            }
        }
        else{
            if(currentRoadIndex > 0){
                //logDebug("increasing curreontRoadIndex");
                currentRoadIndex--;
                ent.getComponent<DestinationBasedMovement>().setDestination(Vec3(roadList[currentRoadIndex].x - 25 + 0.5, pos.y, roadList[currentRoadIndex].y -25 + 0.5));
                ent.getComponent<DestinationBasedMovement>().updatePath();
            }
        }
    }
}


void Jeep::Score(){
    state = travelingBack;
    global->getComponent<GameManager>().addRating(score);
}