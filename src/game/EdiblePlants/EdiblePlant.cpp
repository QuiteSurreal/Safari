#include "EdiblePlant.hpp"

EdiblePlant::EdiblePlant() {

    accumulator = 0.0f;
    timeStep = 1.0f / 60.0f;
    every6FrameTimer = 0.0f;

    alive = true;

    maxFoodValue = 0;
    foodValue = 0;

    consumable = false;
    consumableLimit = 0;

    growing = false;
    maxGrowthCounter = 0;
    growthCounter = maxGrowthCounter;

}


int EdiblePlant::consume(int amount) {
    int consumed = std::min(amount, foodValue);
    foodValue -= consumed;
    if (foodValue <= 0) consumable = false;
    return consumed;
}


void EdiblePlant::initializeDestruction() {
    alive = false;
}


void EdiblePlant::start(Entity& entity) {

    foodValue = maxFoodValue;
    consumable = true;
    growing = false;
    growthCounter = maxGrowthCounter;

}


void EdiblePlant::update(Entity& entity) {

    accumulator += Input::getDeltaTime();

    while (accumulator >= timeStep) {

        // <------ << Timers >> ------>

        accumulator -= timeStep;
        every6FrameTimer += timeStep;

        // >>--------<<


        float foodValuePercentage = static_cast<float>(foodValue) / maxFoodValue;

        if (!consumable) {
            entity.getComponent<Animator>().switchAnimation("depleted");
        }
        else if (foodValuePercentage <= 0.3f) {
            entity.getComponent<Animator>().switchAnimation("scarce");
        }
        else if (foodValuePercentage <= 0.65f) {
            entity.getComponent<Animator>().switchAnimation("half");
        }
        else {
            entity.getComponent<Animator>().switchAnimation("intact");
        }

        if (!alive) {
            destroyEntity();
        }

        // <------ << Every 6 frames >> ------>
        if (every6FrameTimer >= 0.1f) {

            // Grows when under the maximum food value.
             // If the plant grows beyond a certain limit, it becomes consumable again.
            if (growing) {

                if (growthCounter > 0) {
                    growthCounter--;
                }
                else {
                    foodValue += 1;
                    growthCounter = maxGrowthCounter;
                }

                if (!consumable && foodValue >= consumableLimit) {
                    consumable = true;
                }

                if (foodValue >= maxFoodValue) {
                    growing = false;
                }

            }
            else {
                if (foodValue < maxFoodValue) {
                    growing = true;
                }
            }

        }
        // >>--------<<

    }

}


void EdiblePlant::destroyEntity() {
    //if (targetingEntities.size() == 0) entity->destroy();
}



