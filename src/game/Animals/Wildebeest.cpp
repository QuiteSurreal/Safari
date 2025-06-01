#include "Wildebeest.hpp"
#include "WildebeestHerd.hpp"

Wildebeest::Wildebeest() {

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


void Wildebeest::goForFood(Entity& food) {}

int Wildebeest::getFoodPercentage() { return 0; }

void Wildebeest::secondHandSpook(Vec3 source, int spookLevel) {
    sourceOfFear = source;
    spook = spookLevel;
    if (spook > 0) {
        enterSpookedState();
    }
}



void Wildebeest::start(Entity& entity) {

}

void Wildebeest::update(Entity& entity) {

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
        throw std::runtime_error("Unknown animal (Wildebeest) state.");
        break;
    }


}


void Wildebeest::checkThreat() {

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


void Wildebeest::updateAnimalLabel() {
    animalLabel = "wildebeestHerd:" + std::to_string(herdId) + ":" + std::to_string(localId);
}

void Wildebeest::updateHerdLabel() {
    herdLabel = "wildebeestHerd:" + std::to_string(herdId);
}

void Wildebeest::alertOtherAnimals() {

    Entity& herd = Labels::getUnique(herdLabel);
    std::vector<std::string> animals = herd.getComponent<WildebeestHerd>().getAnimalIds();

    for (const std::string& id : animals) {

        Entity& animal = Labels::getUnique(id);
        bool inRange = distance(animal.getComponent<Transform>().getPosition(), entity->getComponent<Transform>().getPosition()) < 1.0f; // TERRIBLE magic number, could be a const or a member, I'll revise it later

        if (inRange) {
            if (animal.getComponent<Wildebeest>().getState() != "Spooked") {
                animal.getComponent<Wildebeest>().secondHandSpook(sourceOfFear, spook);
            }
        }

    }

}



void Wildebeest::enterNeutralState() {
    state = State::Neutral;
    entity->getComponent<DestinationBasedMovement>().setSpeed(0.02f);
    Labels::getUnique(herdLabel).getComponent<WildebeestHerd>().updateUniqueRoaming(*entity);
}

void Wildebeest::enterEnRouteForFoodState() {
    state = State::EnRouteForFood;
}

void Wildebeest::enterEnRouteForWaterState() {
    state = State::EnRouteForWater;
}

void Wildebeest::enterSpookedState() {
    state = State::Spooked;
    entity->getComponent<DestinationBasedMovement>().setSpeed(0.04f - randf(0.0f, 0.015f));
    entity->getComponent<DestinationBasedMovement>().setDestination(-normalize(sourceOfFear-entity->getComponent<Transform>().getPosition()) * 100000.0f); // I totally want to give DestinationBasedMovement a moveIntoDirection method, maybe later.
    entity->getComponent<DestinationBasedMovement>().updatePath();
}

void Wildebeest::enterHornyState() {
    state = State::Horny;
}




