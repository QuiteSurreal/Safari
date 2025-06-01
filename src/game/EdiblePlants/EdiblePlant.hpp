#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <random>
#include "Behaviour.hpp"
#include "Labels.hpp"
#include "../Sprite.hpp"
#include "../AI/ITargetable.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "../Animator.hpp"

class EdiblePlant : public Behaviour {

public:

    // ------------------------------
    // | Constructors
    // ------------------------------

    EdiblePlant();


    // ------------------------------
    // | Public methods
    // ------------------------------

    /// <summary>
    /// Tries to consume food from the food source.
    /// </summary>
    /// <param name="amount">The amount of food to be consumed.</param>
    /// <returns>The amount of food consumed. It can be less than the requested amount if the source is depleted.</returns>
    int consume(int amount);

    /// <summary>
    /// Initializes entity destruction by signaling the entity being dead.
    /// </summary>
    void initializeDestruction();


    // ------------------------------
    // | Overrides
    // ------------------------------

    void start(Entity& entity) override;
    void update(Entity& entity) override;


    // ------------------------------
    // | Getters/setters
    // ------------------------------

    bool isAlive() { return alive; }
    void setAlive(bool value) { alive = value; }

    int getMaxFoodValue() { return maxFoodValue; }
    void setMaxFoodValue(int value) { maxFoodValue = value; }

    bool isConsumable() { return consumable; }
    void setConsumable(bool value) { consumable = value; }

    int getConsumableLimit() { return consumableLimit; }
    void setConsumableLimit(int value) { consumableLimit = value; }

    int getFoodValue() { return foodValue; }
    void setFoodValue(int value) { foodValue = value; }

    bool isGrowing() { return growing; }
    void setGrowing(bool value) { growing = value; }

    int getMaxGrowthCounter() { return maxGrowthCounter; }
    void setMaxGrowthCounter(int value) { maxGrowthCounter = value; }

    int getGrowthCounter() { return growthCounter; }
    void setGrowthCounter(int value) { growthCounter = value; }
 

private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    /// <summary>
    /// Tires to destroy the entity. Succeeds if all targeting entities are removed.
    /// </summary>
    void destroyEntity();


    // ------------------------------
    // | Fields
    // ------------------------------

    // --- timers ---

    float accumulator;
    float timeStep;
    float every6FrameTimer;

    // ---

    std::string plantLabel;

    // --- for ITargetable ---
    bool alive;

    int maxFoodValue;
    int foodValue;

    bool consumable;
    int consumableLimit;    
    bool growing;
    int maxGrowthCounter;
    int growthCounter;

};

