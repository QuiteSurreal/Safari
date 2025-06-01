#include "Lion.hpp"
#include "LionPride.hpp"

Lion::Lion() {

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


void Lion::goForFood(Entity& food) {}

int Lion::getFoodPercentage() { return 0; }

void Lion::secondHandSpook(Vec3 source, int spookLevel) {
    sourceOfFear = source;
    spook = spookLevel;
    if (spook > 0) {
        enterSpookedState();
    }
}



void Lion::start(Entity& entity) {

}

void Lion::update(Entity& entity) {

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
        throw std::runtime_error("Unknown animal (Lion) state.");
        break;
    }


}


void Lion::checkThreat() {

    const auto& set = Labels::getAll("Spooky2");
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


void Lion::updateAnimalLabel() {
    animalLabel = "lionPride:" + std::to_string(herdId) + ":" + std::to_string(localId);
}

void Lion::updateHerdLabel() {
    herdLabel = "lionPride:" + std::to_string(herdId);
}

void Lion::alertOtherAnimals() {

    Entity& herd = Labels::getUnique(herdLabel);
    std::vector<std::string> animals = herd.getComponent<LionPride>().getAnimalIds();

    for (const std::string& id : animals) {

        Entity& animal = Labels::getUnique(id);
        bool inRange = distance(animal.getComponent<Transform>().getPosition(), entity->getComponent<Transform>().getPosition()) < 1.0f; // TERRIBLE magic number, could be a const or a member, I'll revise it later

        if (inRange) {
            if (animal.getComponent<Lion>().getState() != "Spooked") {
                animal.getComponent<Lion>().secondHandSpook(sourceOfFear, spook);
            }
        }

    }

}



void Lion::enterNeutralState() {
    state = State::Neutral;
    entity->getComponent<DestinationBasedMovement>().setSpeed(0.02f);
    Labels::getUnique(herdLabel).getComponent<LionPride>().updateUniqueRoaming(*entity);
}

void Lion::enterEnRouteForFoodState() {
    state = State::EnRouteForFood;
}

void Lion::enterEnRouteForWaterState() {
    state = State::EnRouteForWater;
}

void Lion::enterSpookedState() {
    state = State::Spooked;
    entity->getComponent<DestinationBasedMovement>().setSpeed(0.04f - randf(0.0f, 0.015f));
    entity->getComponent<DestinationBasedMovement>().setDestination(-normalize(sourceOfFear-entity->getComponent<Transform>().getPosition()) * 100000.0f); // I totally want to give DestinationBasedMovement a moveIntoDirection method, maybe later.
    entity->getComponent<DestinationBasedMovement>().updatePath();
}

void Lion::enterHornyState() {
    state = State::Horny;
}




