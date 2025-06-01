#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <random>
//#include "AnimalSite.hpp"
#include "Behaviour.hpp"
#include "Labels.hpp"
#include "Health.hpp"
#include "Core.hpp"
#include "../FaceCamera.hpp"
#include "../HerdManager.hpp"
#include "../Factory.hpp"
#include "../Carcass.hpp"
#include "../Sprite.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "ITargetable.hpp"
#include "Sexes.hpp"

class HerbivoreAI : public Behaviour {

public:

    enum class State {
        Neutral,
        EnRouteForFood,
        EnRouteForWater,
        Spooked,
        Horny
    };


    // ------------------------------
    // | Constructors
    // ------------------------------

    HerbivoreAI();


    // ------------------------------
    // | Public methods
    // ------------------------------

    /// <summary>
    /// Requests a valid food source from the herd and targets it.
    /// </summary>
    /// <returns>A boolean value denoting whether a food source could be targeted.</returns>
    bool askForFood();

    /// <summary>
    /// Requests a valid water source from the herd.
    /// </summary>
    /// <returns>A boolean value denoting whether a water source could be requested.</returns>
    bool askForWater();

    /// <summary>
    /// Returns the animal's satiety as a percentage.
    /// </summary>
    /// <returns>The animal's satiety as a percentage.</returns>
    float calculateFoodPercentage();

    /// <summary>
    /// Returns the animal's hydration as a percentage.
    /// </summary>
    /// <returns>The animal's hydration as a percentage.</returns>
    float calculateWaterPercentage();

    /// <summary>
    /// Returns the animal's spook as a percentage.
    /// </summary>
    /// <returns>The animal's spook as a percentage.</returns>
    float calculateSpookPercentage();

    /// <summary>
    /// Receives a certain level of "spook" directly.
    /// </summary>
    /// <param name="source">The position vector of the source of spook.</param>
    /// <param name="spookLevel">The level of the spook.</param>
    void directSpook(Vec3 source, int spookLevel);

    /// <summary>
    /// Adds a new fear source to the animal's registry.
    /// </summary>
    /// <param name="label">The label denoting the source of fear.</param>
    /// <param name="fearRange">The range in which fear from the specified source will be induced.</param>
    void addSourceOfFear(std::string label, float fearRange);

    /// <summary>
    /// Removes a source of fear.
    /// </summary>
    /// <param name="label">The label denoting the source of fear.</param>
    void removeSourceOfFear(std::string label);

    /// <summary>
    /// Starts breeding with another animal.
    /// </summary>
    /// <param name="other">The "reciving" participant of the action.</param>
    void breed(Entity* other);


    // ------------------------------
    // | Overrides                  
    // ------------------------------

    void start(Entity& entity) override;
    void update(Entity& entity) override;


    // ------------------------------
    // | Getters/setters            
    // ------------------------------

    bool isAlive() const { return alive; }
    void setAlive(bool value) { alive = value; }

    int getHerdId() const { return herdId; }
    void setHerdId(int value) { herdId = value; }

    int getLocalId() const { return localId; }
    void setLocalId(int value) { localId = value; updateAnimalLabel(); }

    std::string getHerdLabel() const { return herdLabel; }
    void setHerdLabel(std::string value) { herdLabel = value; }

    std::string getAnimalLabel() const { return animalLabel; }
    void setAnimalLabel(std::string value) { animalLabel = value; }

    float getBaseSpeed() const { return baseSpeed; }
    void setBaseSpeed(float value) { baseSpeed = value; }

    float getSpeedModifier() const { return speedModifier; }
    void setSpeedModifier(float value) { speedModifier = value; }

    int getIdleMax() const { return idleMax; }
    void setIdleMax(int value) { idleMax = value; }

    int getIdleCounter() const { return idleCounter; }
    void setIdleCounter(int value) { idleCounter = value; }

    std::string getState() const { return stateToString.at(state); }
    void setState(std::string value) { state = stringToState.at(value); }

    bool isAtSite() const { return atSite; }
    void setAtSite(bool value) { atSite = value; }

    int getFoodMax() const { return foodMax; }
    void setFoodMax(int value) { foodMax = value; }

    int getFood() const { return food; }
    void setFood(int value) { food = value; }

    int getWaterMax() const { return waterMax; }
    void setWaterMax(int value) { waterMax = value; }

    int getWater() const { return food; }
    void setWater(int value) { water = value; }

    Vec3 getSourceOfFear() const { return sourceOfFear; }
    void setSourceOfFear(const Vec3& value) { sourceOfFear = value; }

    int getSpookMax() const { return spookMax; }
    void setSpookMax(int value) { spookMax = value; }

    int getSpook() const { return spook; }
    void setSpook(int value) { spook = value; }

    float getSpookPropagationRadius() const { return spookPropagationRadius; }
    void setSpookPropagationRadius(float value) { spookPropagationRadius = value; }

    int getAge() const { return age; }
    void setAge(int value) { age = value; }

    int getAdultAgeFloor() const { return adultAgeFloor; }
    void setAdultAgeFloor(int value) { adultAgeFloor = value; }

    int getRiskyAgeFloor() const { return riskyAgeFloor; }
    void setRiskyAgeFloor(int value) { riskyAgeFloor = value; }

    int getCertainDeathAge() const { return certainDeathAge; }
    void setCertainDeathAge(int value) { certainDeathAge = value; }

    float getAgingCounter() const { return agingCounter; }
    void setAgingCounter(float value) { agingCounter = value; }

    float getAgingCounterMax() const { return agingCounterMax; }
    void setAgingCounterMax(float value) { agingCounterMax = value; }

    Sexes getSex() const { return sex; }
    void setSex(Sexes value) { sex = value; }

    float getBreedingCounter() const { return breedingCounter; }
    void setBreedingCounter(float value) { breedingCounter = value; }

    bool getHasTracker() const { return hasTracker; }
    void setHasTracker(bool value) { hasTracker = value; }


private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    /// <summary>
    /// Queries entities with labels defined in the spook table. Becomes spooked if any of them enter the perimeter specified in the table. The current source of spook is set to the nearest entity.
    /// </summary>
    void checkThreat(); 

    /// <summary>
    /// Updates the animal's label based on its herd's label and the animal's id.
    /// </summary>
    void updateAnimalLabel();

    /// <summary>
    /// Propagates the spooked state to nearby herd members within a specified perimeter.
    /// </summary>
    void alertOtherAnimals(); 

    /// <summary>
    /// Updates the eating behaviour.
    /// </summary>
    /// <returns>A bool value denoting whether the animal could eat.</returns>
    bool updateEating();

    /// <summary>
    /// Updates the drinking behaviour.
    /// </summary>
    /// <returns>A bool value denoting whether the animal could drink.</returns>
    bool updateDrinking();

    /// <summary>
    /// Checks whether the animal is starving (it's food reaches 0). If so, the animal is terminated.
    /// </summary>
    void killIfStarving();

    /// <summary>
    /// Updates the animal's moving speed.
    /// </summary>
    void updateSpeed();

    /// <summary>
    /// Transitions the animal into the Neutral state and performs setup actions associated with the state change.
    /// </summary>
    void enterNeutralState();

    /// <summary>
    /// Transitions the animal into the EnRouteForFood state and performs setup actions associated with the state change.
    /// </summary>
    void enterEnRouteForFoodState();

    /// <summary>
    /// Transitions the animal into the EnRouteForWater state and performs setup actions associated with the state change.
    /// </summary>
    void enterEnRouteForWaterState();

    /// <summary>
    /// Transitions the animal into the Spooked state and performs setup actions associated with the state change.
    /// </summary>
    void enterSpookedState();

    /// <summary>
    /// Transitions the animal into the Horny state and performs setup actions associated with the state change.
    /// </summary>
    void enterHornyState();

    /// <summary>
    /// Kills the animal.
    /// </summary>
    void die();

    /// <summary>
    /// Updates the breeding in progress from the male perspective.
    /// </summary>
    void performBreeding();

    /// <summary>
    /// Bears a baby animal.
    /// </summary>
    void bearNewAnimal();

    /// <summary>
    /// Creates a love marker above the animal.
    /// </summary>
    void createLoveMarker();

    /// <summary>
    /// Hides the love marker above the animal.
    /// </summary>
    void hideLoveMarker();

    /// <summary>
    /// Removes the love marker above the animal.
    /// </summary>
    void removeLoveMarker();

    /// <summary>
    /// Aborts the breeding process and signals the herd of this.
    /// </summary>
    void abortBreeding();


    // ------------------------------
    // | Fields
    // ------------------------------

    // --- for ITargetable --- REMOVE
    bool alive;
    std::vector<Entity* > targetingEntities;
    int targetingEntitiesLimit;
    // -------

    // --- timers ---

    float accumulator;
    float timeStep;

    float everyFrameTimer;
    float every6FrameTimer;
    float every30FrameTimer;
    float every60FrameTimer;

    float opt;
    bool hasTracker;

    // ---

    int herdId;                     // The global identifier of the herd entity.
    int localId;                    // The global identifier of the animal entity.
    std::string herdLabel;          // The herd's label.
    std::string animalLabel;        // The animal's label.

    float baseSpeed;                // The animal's base speed.
    float speedModifier;            // Modifier applied to the animal's base speed.

    int idleMax;                    // Maximum value of the idle counter.
    int idleCounter;                // Keeps track of how long an animal should remain idle.

    State state;                    // The current state of the animal.

    bool atSite;                    // Is the animal's herd stationing at a site.
    Entity* currentFoodSource;      // The food source picked by the animal.
    std::optional<Vec3> currentWaterSource;        // The water source picked by the animal.

    int foodMax;                    // Maximum food level.
    int food;                       // Current food level.
    int waterMax;                   // Maximum water level.
    int water;                      // Current water level.

    std::unordered_map<std::string, float> fearSourceLabels;     // Keeps track of the labels which indicate sources of fear for the animal. It maps these labels to values that specify how close the source has to be to the animal to induce spook.

    Vec3 sourceOfFear;              // Position of the current source of spook.
    int spookMax;                   // Maximum value of the spook level.
    int spook;                      // The level of fear in the animal. If this value reaches zero, the animal is not spooked anymore.
    float spookPropagationRadius;   // The radius inside which the animal will alert other animals of its herd.

    Sexes sex;                      // The animal's sex.

    int age;
    int adultAgeFloor;
    int riskyAgeFloor;
    int certainDeathAge;
    int agingCounter;
    int agingCounterMax;


    Entity* breedingPartner;        // The animal's current breeding partner.
    float breedingCounterMax;       // The maximum value of the breeding counter (duration of the breeding process).
    float breedingCounter;          // Counter for the breeding process.
    Entity* loveMarker;             // The love marker which appears above the horny animals.


    const std::unordered_map<State, std::string> stateToString = {
        {State::Neutral, "Neutral"},
        {State::EnRouteForFood, "EnRouteForFood"},
        {State::EnRouteForWater, "EnRouteForWater"},
        {State::Spooked, "Spooked"},
        {State::Horny, "Horny"}
    };

    const std::unordered_map<std::string, State> stringToState = {
        {"Neutral", State::Neutral},
        {"EnRouteForFood", State::EnRouteForFood},
        {"EnRouteForWater", State::EnRouteForWater},
        {"Spooked", State::Spooked},
        {"Horny", State::Horny}
    };

};

