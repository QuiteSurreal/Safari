#pragma once

#include <queue>
#include <vector>
#include <cmath>
#include <random>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include "Utils.hpp"
#include "../Animals/AnimalSite.hpp"
#include "Behaviour.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "CarnivoreAI.hpp"
#include "HerbivoreAI.hpp"
#include "IHerdRelationshipInteraction.hpp"
#include "Labels.hpp"
#include "Sexes.hpp"
#include "../HerdTypes.hpp"

#define PI 3.14159265358979323846

class CarnivorePackAI : public Behaviour {

public:

    enum class State {
        Wandering,
        Hunting,
        Resting
    };

    // ------------------------------
    // | Constructors
    // ------------------------------

    CarnivorePackAI();


    // ------------------------------
    // | Public methods
    // ------------------------------

    /// <summary>
    /// Calculate positions the animals can take during migation.
    /// </summary>
    void calculateMigratePositions();

    /// <summary>
    /// Adds an animal to the herd. Returns crucial information needed to create the fitting animal entity.
    /// </summary>
    /// <returns>Tuple composed of: 1) the herd's global id, 2) the animal's local id, 3) the animal's label.</returns>
    const std::tuple<int, int, std::string, std::string> addAnimal();

    /// <summary>
    /// Removes an animal from the herd.
    /// </summary>
    /// <param name="id">The animal's local id.</param>
    void removeAnimalFromHerd(int id);

    /// <summary>
    /// Returns all herd's animals' local ids.
    /// </summary>
    /// <returns>Local ids beloning to the herd's animals.</returns>
    std::vector<std::string> getAnimalIds();

    /// <summary>
    /// Updates the roaming position of a specific animal.
    /// </summary>
    /// <param name="ent">The animal entity.</param>
    void updateUniqueRoaming(Entity& ent);

    /// <summary>
    /// Returns a reference to the pack of the prey.
    /// </summary>
    /// <returns>Reference to the prey.</returns>
    Entity* acquirePrey();

    /// <summary>
    /// Picks a valid water source from the map.
    /// </summary>
    /// <returns>The position of the water source if any was found.</returns>
    std::optional<Vec3> pickValidWaterSource();


    // ------------------------------
    // | Overrides
    // ------------------------------

    void start(Entity& entity) override;
    void update(Entity& entity) override;


    // ------------------------------
    // | Getters/setters
    // ------------------------------

    HerdTypes getType() const { return type; }
    void setType(HerdTypes value) { type = value; }

    const std::string& getHerdLabel() const { return herdLabel; }
    void setHerdLabel(const std::string& value) { herdLabel = value; }

    int getGlobalHerdId() const { return globalHerdId; }
    void setGlobalHerdId(int value) { globalHerdId = value; }

    int getMaxHerdSize() const { return maxHerdSize; }
    void setMaxHerdSize(int value) { maxHerdSize = value; }

    int getHerdSize() const { return herdSize; }
    void setHerdSize(int value) { herdSize = value; }

    float getFOVRadius() const { return fovRadius; }
    void setFOVRadius(float value) { fovRadius = value; }

    float getRoamRadius() const { return roamRadius; }
    void setRoamRadius(float value) { roamRadius = value; }

    float getMigrateRadius() const { return migrateRadius; }
    void setMigrateRadius(float value) { migrateRadius = value; }

    float getGeneralMood() const { return generalMood; }
    void setGeneralMood(float value) { generalMood = value; }

    float getOverallSatiety() const { return overallSatiety; }
    void setOverallSatiety(float value) { overallSatiety = value; }

    float getMinimumSatiety() const { return minimumSatiety; }
    void setMinimumSatiety(float value) { minimumSatiety = value; }

    float getSatietyComfortLimit() const { return satietyComfortLimit; }
    void setSatietyComfortLimit(float value) { satietyComfortLimit = value; }

    float getSatietyCriticalLimit() const { return satietyCriticalLimit; }
    void setSatietyCriticalLimit(float value) { satietyCriticalLimit = value; }

    float getHydrationComfortLimit() const { return hydrationComfortLimit; }
    void setHydrationComfortLimit(float value) { hydrationComfortLimit = value; }

    float getHydrationCriticalLimit() const { return hydrationCriticalLimit; }
    void setHydrationCriticalLimit(float value) { hydrationCriticalLimit = value; }

    State getState() const { return state; }
    void setState(State value) { state = value; }

    int getMaxRestcounter() const { return maxRestCounter; }
    void setMaxRestcounter(int value) { maxRestCounter = value; }

    int getRestcounter() const { return restCounter; }
    void setRestcounter(int value) { restCounter = value; }

    bool isFrightened() const { return frightened; }
    void setFrightened(bool value) { frightened = value; }

    int getFrightenedThreshold() const { return frightenedThreshold; }
    void setFrightenedThreshold(int value) { frightenedThreshold = value; }

    int getFrightenedMeter() const { return frightenedMeter; }
    void setFrightenedMeter(int value) { frightenedMeter = value; }

    int getMaxReproductionCounter() const { return maxReproductionCounter; }
    void setMaxReproductionCounter(int value) { maxReproductionCounter = value; }

    int getReproductionCounter() const { return reproductionCounter; }
    void setReproductionCounter(int value) { reproductionCounter = value; }

    bool isReproductionInProgress() const { return reproductionInProgress; }
    void setReproductionInProgress(bool value) { reproductionInProgress = value; }


private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    /// <summary>
    /// Calculates the herd's general mood based on different factors.
    /// </summary>
    void calculateGeneralMood();

    /// <summary>
    /// Calculates the average animal satiety as well as the minimum value.
    /// </summary>
    void assessAnimalSatiety();

    /// <summary>
    /// Handles animal roaming when the herd is stationed at a site. Checks the animal's idle counter and assign them new destinations if needed.
    /// </summary>
    void updateAnimalRoaming();

    /// <summary>
    /// Returns a random position inside the herd's roaming perimeter.
    /// </summary>
    /// <returns>A random position vector inside the herd's roaming perimeter.</returns>
    Vec3 getRandomAnimalRoamingPosition();

    /// <summary>
    /// Updates the migration positions in accordance with the herd entity's movement.
    /// </summary>
    /// <param name="entity">Probably not necessary, I was drunk when I wrote it.</param>
    void updateAnimalMigration(Entity& entity);

    /// <summary>
    /// Updates the herd's reproduction behaviour. The herd might choose instances to mate when conditions are met.
    /// </summary>
    void updateReproduction();

    /// <summary>
    /// Chooses two instances of the opposite sex to mate.
    /// </summary>
    void initiateMating();

    /// <summary>
    /// Notifies animals that the herd is near a prey or not.
    /// </summary>
    /// <param name="atSite">Boolean value denoting whether the herd is stationing near a prey.</param>
    void signalNearPreyToAnimals(bool atSite);

    /// <summary>
    /// Transitions the herd into the Wandering state and performs setup actions associated with the state change.
    /// </summary>
    void enterWanderingState();

    /// <summary>
    /// Transitions the herd into the HeadingToPreferred state and performs setup actions associated with the state change.
    /// </summary>
    void enterHuntingState();

    /// <summary>
    /// Transitions the herd into the Resting state and performs setup actions associated with the state change.
    /// </summary>
    void enterRestingState();

    /// <summary>
    /// Picks a water source.
    /// </summary>
    /// <returns>Water source.</returns>
    std::optional<Vec3> pickWaterSource();


    // ------------------------------
    // | Fields
    // ------------------------------

    // --- timers ---

    float accumulator;
    float timeStep;

    float everyFrameTimer;
    float every6FrameTimer;
    float every30FrameTimer;
    float every60FrameTimer;

    // ---

    HerdTypes type;

    std::uint32_t throttleCounter;  // Controls the behaviour throttle.

    int globalHerdId;               // The herd's global id (herd type specific)
    std::string herdLabel;          // The herd's label.

    int maxHerdSize;                // The maximum size of the herd.
    int herdSize;                   // The number of animal's currently in the herd.

    float fovRadius;                // The herd's field of view.
    float roamRadius;               // The radius of the perimeter inside which the animals can roam freely when the herd is stationary. 
    float migrateRadius;            // The radius of the perimeter inside which the animals will take position when the herd is moving.

    float generalMood;              // The herd's general mood, affected by multiple factors.

    float overallSatiety;           // The herd's overall satiety, calculated from the satiety of the individual animals.
    float minimumSatiety;           // The herd's minimum satiety.

    float overallHydration;           // The herd's overall hydration, calculated from the hydration of the individual animals.
    float minimumHydration;           // The herd's minimum hydration.

    float satietyComfortLimit;      // If the overall satiety falls under this limit, the herd starts looking for a site to eat.
    float satietyCriticalLimit;     // If at least one animal's satiety falls under this limit, the herd starts looking for a site to eat.

    float hydrationComfortLimit;    // If the overall satiety falls under this limit, the herd starts looking for a site to drink.
    float hydrationCriticalLimit;   // If at least one animal's hydration falls under this limit, the herd starts looking for a site to drink.

    bool atSite;                    // Denotes whether the herd is at a site, nourishing themselves.

    float neighborEffect;           // Effect applied to the herd's mood based on its attitude toward its neighbors.
    std::unordered_map<std::string, float> speciesRelationships;    // The registry of species that affect the general mood of the herd.

    State state;                    // The herd's current state.

    int maxRestCounter;             // Maximum restfulness.
    int restCounter;                // The value representing how rested the herd is. 0 means fatigue.

    bool frightened;                // Indicates if the herd itself is considered frightened. 
    int frightenedThreshold;        // [TODO] The threshold above which the herd is considered frightened.
    int frightenedMeter;            // [TODO] Indicates how frightened the herd is.

    std::vector<Vec3> herdMigratePositions;     // The positions that the animals will take during migration.

    int maxReproductionCounter;     // Maximum value of the reproductionCounter
    int reproductionCounter;        // When the reproduction counter hits zero, the herd will choose animal instances to mate.
    bool reproductionInProgress;    // Denotes whether business is in progress.


    Entity* commonPrey;


};

