#pragma once

#include <random>
#include <algorithm>
#include "Utils.hpp"
#include "Behaviour.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"

class RandomMigration : public Behaviour {

public:

    // ------------------------------
    // | Constructors
    // ------------------------------

    RandomMigration();


    // ------------------------------
    // | Public methods
    // ------------------------------



    // ------------------------------
    // | Overrides
    // ------------------------------

    void start(Entity& entity) override;
    void update(Entity& entity) override;


    // ------------------------------
    // | Getters/setters
    // ------------------------------

    


private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    


    // ------------------------------
    // | Fields
    // ------------------------------

    int counterMax;
    int counter;

};

