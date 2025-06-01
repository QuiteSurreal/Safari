#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <random>
#include "AnimalSite.hpp"
#include "Behaviour.hpp"
#include "Labels.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"

class Lion : public Behaviour {

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

    Lion();


    // ------------------------------
    // | Public methods
    // ------------------------------

    void goForFood(Entity& food);
    int getFoodPercentage();
    void secondHandSpook(Vec3 source, int spookLevel);


    // ------------------------------
    // | Overrides
    // ------------------------------

    void start(Entity& entity) override;
    void update(Entity& entity) override;


    // ------------------------------
    // | Getters/setters
    // ------------------------------

    int getHerdId() const { return herdId; }
    void setHerdId(int value) { herdId = value; updateHerdLabel(); }

    int getLocalId() const { return localId; }
    void setLocalId(int value) { localId = value; updateAnimalLabel(); }

    int getIdleMax() const { return idleMax; }
    void setIdleMax(int value) { idleMax = value; }

    int getIdleCounter() const { return idleCounter; }
    void setIdleCounter(int value) { idleCounter = value; }

    std::string getState() const { return stateToString.at(state); }
    void setState(std::string value) { state = stringToState.at(value); }

    float getFieldOfView() const { return fieldOfView; }
    void setFieldOfView(float value) { fieldOfView = value; }

    Vec3 getSourceOfFear() const { return sourceOfFear; }
    void setSourceOfFear(const Vec3& value) { sourceOfFear = value; }

    int getSpookMax() const { return spookMax; }
    void setSpookMax(int value) { spookMax = value; }

    int getSpook() const { return spook; }
    void setSpook(int value) { spook = value; }


private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    void checkThreat(); 
    void updateAnimalLabel();
    void updateHerdLabel();
    void alertOtherAnimals(); 

    void enterNeutralState();
    void enterEnRouteForFoodState();
    void enterEnRouteForWaterState();
    void enterSpookedState();
    void enterHornyState();


    // ------------------------------
    // | Fields
    // ------------------------------

    int herdId;
    int localId;
    std::string animalLabel;
    std::string herdLabel;

    int idleMax;
    int idleCounter;

    State state;

    Entity* currentFoodSource;
    int foodMax;
    int food;

    float fieldOfView;
    Vec3 sourceOfFear;
    int spookMax;
    int spook;


    // CURSED, consult the bois
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

