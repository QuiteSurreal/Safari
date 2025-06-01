#include "CarnivoreAI.hpp"
#include "CarnivorePackAI.hpp"

CarnivoreAI::CarnivoreAI() {

    accumulator = 0.0f;
    timeStep = 1.0f / 60.0f;
    everyFrameTimer = 0.0f;
    every6FrameTimer = 0.0f;
    every30FrameTimer = 0.0f;
    every60FrameTimer = 0.0f;

    herdId = 0;
    localId = 0;
    herdLabel = "none";
    animalLabel = "none";

    baseSpeed = 0;
    speedModifier = 1;

    idleMax = 0;
    idleCounter = 0;

    opt = 0.0f;
    hasTracker = false;

    state = State::Neutral;

    atSite = false;
    currentFoodSource = nullptr;
    currentWaterSource = Vec3(0.0f, 0.0f, 0.0f);

    foodMax = 0;
    food = 0;
    waterMax = 0;
    water = 0;

    sourceOfFear = Vec3(0.0f, 0.0f, 0.0f);
    spookMax = 0;
    spook = 0;
    spookPropagationRadius = 0.0f;

    // TEMP
    alive = true;
    targetingEntitiesLimit = 10;
    // TEMP

    age = 0;
    adultAgeFloor = 0;
    riskyAgeFloor = 0;
    certainDeathAge = 0;
    agingCounter = 0;
    agingCounterMax = 0;


    sex = Sexes::Female; 
    breedingCounterMax = 800;
    breedingCounter = 0;

    breedingPartner = nullptr;
    
    loveMarker = nullptr;

}


bool CarnivoreAI::askForFood() {

    if (atSite) {
        Entity* foodSource = Labels::getUnique(herdLabel).getComponent<CarnivorePackAI>().acquirePrey();

        if (!foodSource)
            return false;

        currentFoodSource = foodSource;
        return true;
    }
    else {
        return false;
    }

}


bool CarnivoreAI::askForWater() {

    std::optional<Vec3> waterSource = Labels::getUnique(herdLabel).getComponent<CarnivorePackAI>().pickValidWaterSource();

    if (!waterSource)
        return false;

    currentWaterSource = waterSource.value();

    return true;

}


float CarnivoreAI::calculateFoodPercentage() {
    return (foodMax == 0) ? 0.0f : static_cast<float>(food) / foodMax;
}

float CarnivoreAI::calculateWaterPercentage() {
    return (waterMax == 0) ? 0.0f : static_cast<float>(water) / waterMax;
}


float CarnivoreAI::calculateSpookPercentage() {
    return (spookMax == 0) ? 0.0f : static_cast<float>(spook) / spookMax;
}


void CarnivoreAI::directSpook(Vec3 source, int spookLevel) {
    sourceOfFear = source;
    spook = spookLevel;
    if (spook > 0) {
        enterSpookedState();
    }
}


void CarnivoreAI::addSourceOfFear(const std::string label, float fearRange) {
    fearSourceLabels[label] = fearRange;
}


void CarnivoreAI::removeSourceOfFear(const std::string label) {
    fearSourceLabels.erase(label);
}


void CarnivoreAI::breed(Entity* other) {

    if (other->getComponent<Targetable>().target()) {

        breedingPartner = other;
        breedingPartner->getComponent<CarnivoreAI>().enterHornyState();

        enterHornyState();

    }

}


void CarnivoreAI::start(Entity& entity) {

    entity.getComponent<Animator>().switchAnimation("idle");

    food = foodMax;
    water = waterMax;

    updateSpeed();

}


void CarnivoreAI::update(Entity& entity) {

    accumulator += Input::getDeltaTime();

    while (accumulator >= timeStep) {

        // <------ << Timers >> ------>

        accumulator -= timeStep;
        everyFrameTimer += timeStep;
        every6FrameTimer += timeStep;
        every30FrameTimer += timeStep;
        every60FrameTimer += timeStep;

        // >>--------<<

        
        // <------ << Every frame >> ------>>

        //update speed

        entity.getComponent<DestinationBasedMovement>().setSpeed(Labels::getUnique("global").getComponent<GameManager>().getSpeed() * 0.02f); 

        // |-------------------------------------------------------------|
        // |###| Die |###################################################|
        // |-------------------------------------------------------------|

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
                Labels::getUnique("global").getComponent<GameManager>().removeAnimal();
		    	die();
                return;
		    }
	    }



        // |-------------------------------------------------------------|
        // |###| Animation controller |##################################|
        // |-------------------------------------------------------------|

        if (entity.getComponent<DestinationBasedMovement>().getArrived()) {
            entity.getComponent<Animator>().switchAnimation("idle");
        }
        else {
            entity.getComponent<Animator>().switchAnimation("running");
        }

        if ((Labels::getUnique("global").getComponent<Time>().getHours() >= 21 || Labels::getUnique("global").getComponent<Time>().getHours() <= 3) && !hasTracker)
        {
            opt += 0.01f * Labels::getUnique("global").getComponent<GameManager>().getSpeed();
            entity.getComponent<Sprite>().setOpacity(opt);
        }
        else if (opt > 0.0f)
        {
            opt = 0.0f;
            entity.getComponent<Sprite>().setOpacity(opt);
        }



        // |-------------------------------------------------------------|
        // |###| Aging related |#########################################|
        // |-------------------------------------------------------------|

        // <------ << Every 6 frames >> ------>
        if (every6FrameTimer >= 0.1f) {

            // Size depending on the age of the animal.
            if (age >= adultAgeFloor) {
                entity.getComponent<Transform>().scale(1.45f);
            }
            else {
                entity.getComponent<Transform>().scale(0.95f);
            }

            agingCounter -= 1.0f;

            if (agingCounter <= 0) {
                age += 1;
                agingCounter = agingCounterMax;
            }

            // The risk of dying increases with age after reaching riskyAgeFloor.
            // Becomes 100% when entering certainDeathAge.
            if (age >= riskyAgeFloor)
            {
                float t = (age - riskyAgeFloor) / static_cast<float>(certainDeathAge - riskyAgeFloor);
                t = std::clamp(t, 0.0f, 1.0f);

                if (randf(0.0f, 1.0f) < t)
                {
                    logDebug("Dead");
                }
            }

        }
        // >>--------<<


        
        // |-------------------------------------------------------------|
        // |###| Satiety and hydration |#################################|
        // |-------------------------------------------------------------|
        
        // <------ << Every 6 frames >> ------>
        if (every6FrameTimer >= 0.1f) {

            if (food <= 0 || water <= 0) {
                killIfStarving(); // rewrite this
            }
            else {
                food -= 1;
                water -= 1;
            }
            
        }
        // >>--------<<
        
 

        // |-------------------------------------------------------------|
        // |###| Other updates |#########################################|
        // |-------------------------------------------------------------|

        checkThreat();



        // |-------------------------------------------------------------|
        // |###| Main state machine |####################################|
        // |-------------------------------------------------------------|

        switch (state) {

        case State::Neutral: {

            if (!atSite) {

                //enterNeutralState();

                // <------ << Every 6 frames >> ------>
                if (every6FrameTimer >= 0.1f) {
                    if (idleCounter > 0) {
                        idleCounter -= 1;
                    }
                }
                // >>--------<<

            }
            else {

                float foodPercent = calculateFoodPercentage();
                float waterPercent = calculateWaterPercentage();

                if (foodPercent < 0.9f) {  // MAGIC NUMBER
                    enterEnRouteForFoodState();
                }
                else if (waterPercent < 0.9f) { // MAGIC NUMBER
                    enterEnRouteForWaterState();
                }

            }

            break;

        }

        case State::EnRouteForFood: {

            if (!currentFoodSource) {
                enterNeutralState();
                break;
            }

            bool foodSourceStillValid = updateEating();

            if (!foodSourceStillValid) {
                enterNeutralState();
                break;
            }

            float foodPercent = calculateFoodPercentage();
            if (foodPercent > 0.98f) {  // MAGIC NUMBER
                enterNeutralState();
                break;
            }

            break;

        }

        case State::EnRouteForWater: {

            if (!currentWaterSource) {
                enterNeutralState();
                break;
            }

            bool waterSourceStillValid = updateDrinking();

            if (!waterSourceStillValid) {
                enterNeutralState();
                break;
            }

            float waterPercent = calculateWaterPercentage();
            if (waterPercent > 0.98f) {  // MAGIC NUMBER
                enterNeutralState();
                break;
            }

            break;

        }

        case State::Spooked:

            // [ TODO ] we might need some other settings here if eating or drinking was aborted???????

            // If breeding was in progress when the animals got spooked, then we need to run
            // the performBreeding method one more time to make sure that the breeding sequence closes properly.
            if (breedingPartner != nullptr) {
                performBreeding();
            }

            alertOtherAnimals();

            // <------ << Every 6 frames >> ------>
            if (every6FrameTimer >= 0.1f) {
                if (spook > 0) {
                    spook -= 1;
                }
                else {
                    enterNeutralState();
                }
            }
            // >>--------<<

            break;

        case State::Horny:

            if (sex == Sexes::Male) {
                performBreeding();
            }

            break;

        default:
            throw std::runtime_error("Unknown animal state.");
            break;
        }



        // <------ << Timer handling >> ------>

        if (every6FrameTimer >= 0.1f) 
            every6FrameTimer -= 0.1f;
       
        if (every30FrameTimer >= 0.5f)
            every30FrameTimer -= 0.5f;

        if (every60FrameTimer >= 1.0f)
            every60FrameTimer -= 1.0f;

        // >>--------<<

    }

}


void CarnivoreAI::checkThreat()
{

    Entity* bestSource = nullptr;
    float bestDist = std::numeric_limits<float>::max();

    // Check all sources of fear.
    for (const auto& [label, fearDistance] : fearSourceLabels)
    {

        const auto& entitySet = Labels::getAll(label);
        if (entitySet.empty()) {
            continue;
        }

        // Check each entity with this label. If the entity is inside the specified perimeter
        // and is closer than the previously found closest source of fear, update the latter.
        for (Entity* e : entitySet)
        {
            float d = distance(
                e->getComponent<Transform>().getPosition(),
                this->entity->getComponent<Transform>().getPosition()
            );

            if (d < fearDistance && d < bestDist)
            {
                bestDist = d;
                bestSource = e;
            }
        }
    }

    if (bestSource)
    {
        sourceOfFear = bestSource->getComponent<Transform>().getPosition();
        spook = spookMax;
        enterSpookedState();
    }
   
}


void CarnivoreAI::updateAnimalLabel() {
    animalLabel = herdLabel + ":" + std::to_string(localId);
}


void CarnivoreAI::alertOtherAnimals() {

    Entity& herd = Labels::getUnique(herdLabel);
    std::vector<std::string> animals = herd.getComponent<CarnivorePackAI>().getAnimalIds();

    // Query fellow animals from the herd and give "second hand spook" those inside the spook propagation radius.
    for (const std::string& id : animals) {

        Entity& animal = Labels::getUnique(id);
        bool inRange = distance(animal.getComponent<Transform>().getPosition(), entity->getComponent<Transform>().getPosition()) < spookPropagationRadius;

        if (inRange) {
            if (animal.getComponent<CarnivoreAI>().getState() != "Spooked") {
                animal.getComponent<CarnivoreAI>().directSpook(sourceOfFear, spook);
            }
        }

    }

}


bool CarnivoreAI::updateEating() {

    // Check if the plant is still consumable.
    if (!currentFoodSource) {
        return false;
    }

    // Go next to the food source.
    float angle = randf(0.0f, 2.0f * 3.1415926f);
    float distance = 1.0f;
    Vec3 offset = Vec3(std::cos(angle) * distance, 0.0f, std::sin(angle) * distance);
    Vec3 destPos = currentFoodSource->getComponent<Transform>().getPosition() + offset;
    DestinationBasedMovement& destMov = getEntity().getComponent<DestinationBasedMovement>();
    destMov.setDestination(destPos);
    destMov.updatePath();

    // If close enough, eat
    if (glm::distance(getEntity().getComponent<Transform>().getPosition(), currentFoodSource->getComponent<Transform>().getPosition()) < 1.02f) {
        food = foodMax;
        currentFoodSource = nullptr;
        return false;
    }

    return true;

}


bool CarnivoreAI::updateDrinking() {

    // Go next to the water source.
    float angle = randf(0.0f, 2.0f * 3.1415926f);
    float distance = 1.0f;
    Vec3 offset = Vec3(std::cos(angle) * distance, 0.0f, std::sin(angle) * distance);
    Vec3 destPos = currentWaterSource.value() + offset;
    DestinationBasedMovement& destMov = getEntity().getComponent<DestinationBasedMovement>();
    destMov.setDestination(destPos);
    destMov.updatePath();

    // If close enough, drink
    if (glm::distance(getEntity().getComponent<Transform>().getPosition(), currentWaterSource.value()) < 1.02f) {
        water += 60; 
    }

    return true;

}


void CarnivoreAI::killIfStarving() {
    entity->getComponent<Health>().decreaseHealth(1);
    // TODO: kill the animal
    //logDebug("I am starving and I should be dead. :(");
}


void CarnivoreAI::updateSpeed() {
    entity->getComponent<DestinationBasedMovement>().setSpeed(baseSpeed * speedModifier);
}

void CarnivoreAI::enterNeutralState() {
    state = State::Neutral;
    speedModifier = 1.0f;
    updateSpeed();
    Labels::getUnique(herdLabel).getComponent<CarnivorePackAI>().updateUniqueRoaming(*entity);
}

void CarnivoreAI::enterEnRouteForFoodState() {
    bool success = askForFood();
    //if (!success)
        //logDebug("This should not happen: the animal could not request food.");
    state = State::EnRouteForFood;
}

void CarnivoreAI::enterEnRouteForWaterState() {
    bool success = askForWater();
    if (!success) {
        logDebug("Could not request water, unalive mehhh.");
    }
    state = State::EnRouteForWater;
}

void CarnivoreAI::enterSpookedState() {
    state = State::Spooked;
    speedModifier = 1.4f + randf(0.0f, 0.2f);
    updateSpeed();
    // Sends animals to the direction opposite to the source of fear.
    entity->getComponent<DestinationBasedMovement>().setDestination(-normalize(sourceOfFear-entity->getComponent<Transform>().getPosition()) * 100000.0f);
    entity->getComponent<DestinationBasedMovement>().updatePath();
}

void CarnivoreAI::enterHornyState() {

    state = State::Horny;
    createLoveMarker();

    if (sex == Sexes::Male) {
        breedingCounter = breedingCounterMax;
        speedModifier = 1.4f;
        updateSpeed();
    }

}

void CarnivoreAI::die()
{
    //should abort breeding yk
    Entity herd = Labels::getUnique(herdLabel);
    herd.getComponent<CarnivorePackAI>().removeAnimalFromHerd(localId);
    const Texture texture = Texture::fromImage("redGrass.png");
    Factory::createCarcass(entity->getComponent<Transform>().getPosition(), entity->getComponent<Health>().getKiller(), texture);
    entity->destroy();

}


void CarnivoreAI::performBreeding() {

    // DEBUG
    if (!breedingPartner) {
        //throw std::runtime_error("No partner found.");
        return;
    }

    // Checks if the partner ceased to be available.
    if (!breedingPartner->getComponent<Targetable>().canTarget()) {
        breedingPartner->getComponent<Targetable>().removeTargeting();
        abortBreeding();
        return;
    }

    // Checks if the partner is still horny.
    if (breedingPartner->getComponent<CarnivoreAI>().getState() != "Horny") {
        abortBreeding();
        return;
    }

    // Checks if the breeding is still in progress.
    if (breedingCounter >= 0) {

        breedingCounter -= 1.0f;

        // Following/approaching the partner
        if (!breedingPartner->getComponent<DestinationBasedMovement>().getArrived()) {
            Vec3 partnerPos = breedingPartner->getComponent<Transform>().getPosition();
            Vec3 partnerDir = glm::normalize(breedingPartner->getComponent<DestinationBasedMovement>().getDestination() - partnerPos + Vec3(0.01f, 0.0f, 0.0f)); // TEMPORARY
            Vec3 breedingPos = partnerPos + -partnerDir * 0.4f;
            getEntity().getComponent<DestinationBasedMovement>().setDestination(breedingPos);
            getEntity().getComponent<DestinationBasedMovement>().updatePath();
        }
        else {
            Vec3 partnerPos = breedingPartner->getComponent<Transform>().getPosition();
            getEntity().getComponent<DestinationBasedMovement>().setDestination(partnerPos + Vec3(0.4f, 0.0f, 0.4f));
            getEntity().getComponent<DestinationBasedMovement>().updatePath();
        }

    }
    else {
        // The breeding session is over.
        breedingPartner->getComponent<CarnivoreAI>().bearNewAnimal();
        abortBreeding();
    }

}


void CarnivoreAI::bearNewAnimal() {

    Entity& baby = Labels::getUnique("eco").getComponent<HerdManager>().addAnimalToHerd(Labels::getUnique(herdLabel).getComponent<CarnivorePackAI>().getType(), sex, herdId);
    baby.getComponent<CarnivoreAI>().setAge(0);
    baby.getComponent<Transform>().relocate(getEntity().getComponent<Transform>().getPosition());
}


void CarnivoreAI::createLoveMarker() {

    if (loveMarker) {
        loveMarker->getComponent<Sprite>().setOpacity(0);
    }
    else {
        Entity& entity = Core::createEntity();
        Transform& trans = entity.addComponent<Transform>();
        entity.addComponent<FaceCamera>();

        trans.setParent(getEntity().getComponent<Transform>());
        trans.translate(Vec3(0.0f, 1.0f, 0.0f));

        const Texture texture = Texture::fromImage("heartTemp.png");
        entity.addComponent(Sprite{ trans, texture });

        loveMarker = &entity;
    }

}


void CarnivoreAI::hideLoveMarker() {
    if (loveMarker)
        loveMarker->getComponent<Sprite>().setOpacity(1);
}


void CarnivoreAI::removeLoveMarker() {
    loveMarker->destroy();
    loveMarker = nullptr;
}


void CarnivoreAI::abortBreeding() {

    //hideLoveMarker();
    removeLoveMarker();
    enterNeutralState();

    // For the partner.
    if (breedingPartner->getComponent<Targetable>().canTarget()) {
        //breedingPartner->getComponent<CarnivoreAI>().hideLoveMarker();
        breedingPartner->getComponent<CarnivoreAI>().removeLoveMarker();
        breedingPartner->getComponent<CarnivoreAI>().enterNeutralState();
        breedingPartner->getComponent<Targetable>().removeTargeting();
    }

    breedingPartner = nullptr;

    // Notifying the herd.
    CarnivorePackAI& ai = Labels::getUnique(herdLabel).getComponent<CarnivorePackAI>();
    ai.setReproductionInProgress(false);

}

