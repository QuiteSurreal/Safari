#include "Giraffe.hpp"
#include "GiraffeHerd.hpp"

Giraffe::Giraffe() {

    herdId = 0;
    localId = 0;
    animalLabel = "none";
    herdLabel = "none";

    idleMax = 0;
    idleCounter = 0;

    state = State::Neutral;

    currentFoodSource = nullptr;
    foodMax = 0;
    food = 0;

    fieldOfView = 0;
    sourceOfFear = Vec3(0.0f, 0.0f, 0.0f);
    spookMax = 0;
    spook = 0;

}


void Giraffe::goForFood(Entity& food) {}

int Giraffe::getFoodPercentage() { return 0; }

void Giraffe::secondHandSpook(Vec3 source, int spookLevel) {
    sourceOfFear = source;
    spook = spookLevel;
    if (spook > 0) {
        enterSpookedState();
    }
}



void Giraffe::start(Entity& entity) {

}

void Giraffe::update(Entity& entity) {

    checkThreat();

    switch (state) {

    case State::Neutral: {

        // TEMP
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<int> dist(0, 100);
        // TEMP

        if (idleCounter > 0) {
            idleCounter -= dist(gen);
        }

        // Destination based movement should beeee here (maybe later)

        break;

    }

    case State::EnRouteForFood:
        // en route to k�nai
        break;

    case State::EnRouteForWater:
        // en route to piny�
        break;

    case State::Spooked:
        
        if (spook > 0) {
            alertOtherAnimals();
            spook -= 1;
        }
        else {
            enterNeutralState();
        }

        break;

    case State::Horny:
        // time for dug�s
        break;

    default:
        throw std::runtime_error("Unknown animal (Giraffe) state.");
        break;
    }


}


void Giraffe::checkThreat() {

    const auto& set = Labels::getAll("Spooky");
    std::vector<Entity*> spookies(set.begin(), set.end());

    /* BRUH
    std::vector<Entity*> inVicinity;
    std::copy_if(spookies.begin(), spookies.end(), std::back_inserter(inVicinity),
        [&](Entity* e) { return distance(e->getComponent<Transform>().getPosition(), this->entity->getComponent<Transform>().getPosition()) < fieldOfView ; });
    */

    Entity* closest = nullptr;
    float minDist = std::numeric_limits<float>::max();

    for (Entity* e : spookies) {
        float d = distance(
            e->getComponent<Transform>().getPosition(),
            this->entity->getComponent<Transform>().getPosition());

        if (d < minDist) {
            minDist = d;
            closest = e;
        }
    }

    if (minDist >= fieldOfView) {
        closest = nullptr;
    }
    else {
        sourceOfFear = closest->getComponent<Transform>().getPosition();
        spook = spookMax;
        enterSpookedState();
    }

}


void Giraffe::updateAnimalLabel() {
    animalLabel = "giraffeHerd:" + std::to_string(herdId) + ":" + std::to_string(localId);
}

void Giraffe::updateHerdLabel() {
    herdLabel = "giraffeHerd:" + std::to_string(herdId);
}

void Giraffe::alertOtherAnimals() {

    Entity& herd = Labels::getUnique(herdLabel);
    std::vector<std::string> animals = herd.getComponent<GiraffeHerd>().getAnimalIds();

    for (const std::string& id : animals) {

        Entity& animal = Labels::getUnique(id);
        bool inRange = distance(animal.getComponent<Transform>().getPosition(), entity->getComponent<Transform>().getPosition()) < 1.0f; // TERRIBLE magic number, could be a const or a member, I'll revise it later

        if (inRange) {
            if (animal.getComponent<Giraffe>().getState() != "Spooked") {
                animal.getComponent<Giraffe>().secondHandSpook(sourceOfFear, spook);
            }
        }

    }

}



void Giraffe::enterNeutralState() {
    state = State::Neutral;
    entity->getComponent<DestinationBasedMovement>().setSpeed(0.02f);
    Labels::getUnique(herdLabel).getComponent<GiraffeHerd>().updateUniqueRoaming(*entity);
}

void Giraffe::enterEnRouteForFoodState() {
    state = State::EnRouteForFood;
}

void Giraffe::enterEnRouteForWaterState() {
    state = State::EnRouteForWater;
}

void Giraffe::enterSpookedState() {
    state = State::Spooked;
    entity->getComponent<DestinationBasedMovement>().setSpeed(0.04f - randf(0.0f, 0.015f));
    entity->getComponent<DestinationBasedMovement>().setDestination(-normalize(sourceOfFear-entity->getComponent<Transform>().getPosition()) * 100000.0f); // I totally want to give DestinationBasedMovement a moveIntoDirection method, maybe later.
    entity->getComponent<DestinationBasedMovement>().updatePath();
}

void Giraffe::enterHornyState() {
    state = State::Horny;
}




