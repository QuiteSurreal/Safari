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
#include "AnimalSite.hpp"
#include "Behaviour.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "Lion.hpp"
#include "Labels.hpp"

#define PI 3.14159265358979323846

class LionPride : public Behaviour {

public:

    enum class State {
        Wandering,
        HeadingToPreferred,
        Resting
    };

    // ------------------------------
    // | Constructors
    // ------------------------------

    LionPride();


    // ------------------------------
    // | Public methods
    // ------------------------------

    void updateState();
    bool areAnimalsSatisfied();
    void assessThreat();
    void assessNeighbors();
    void calculateMigratePositions();
    void assignUniqueHerdId(int id);
    const std::tuple<int, int, std::string> addAnimal();
    void removeAnimalFromHerd(int id);
    std::vector<std::string> getAnimalIds();
    void updateUniqueRoaming(Entity& ent);

    // ------------------------------
    // | Overrides
    // ------------------------------

    void start(Entity& entity) override;
    void update(Entity& entity) override;


    // ------------------------------
    // | Getters/setters
    // ------------------------------

    int getGlobalHerdId() const { return globalHerdId; }
    const std::string& getHerdLabel() const { return herdLabel; }

    int getHerdSize() const { return herdSize; }

    float getFOVRadius() const { return fovRadius; }
    void setFOVRadius(float value) { fovRadius = value; }

    float getRoamRadius() const { return roamRadius; }
    void setRoamRadius(float value) { roamRadius = value; }

    float getMigrateRadius() const { return migrateRadius; }
    void setMigrateRadius(float value) { migrateRadius = value; }


private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    void updateAnimalRoaming();
    Vec3 getRandomAnimalRoamingPosition();
    void updateAnimalMigration(Entity& entity);


    // ------------------------------
    // | Fields
    // ------------------------------

    int globalHerdId;
    std::string herdLabel;

    int herdSize;

    //std::priority_queue<AnimalSite*, std::vector<AnimalSite*>, SiteComparator> PreferredSites;
    //std::priority_queue<AnimalSite*, std::vector<AnimalSite*>, SiteComparator> FavouriteSites;

    float fovRadius;
    float roamRadius;
    float migrateRadius;
    float generalMood; // TODO

    State state;
    int restMeter; // TODO
    bool frightened; // TODO
    int frightenedMeter; // TODO
    int neighborEffect; // TODO

    std::vector<Vec3> herdMigratePositions;

};

