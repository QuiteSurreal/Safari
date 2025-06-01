#pragma once

#include "Utils.hpp"
#include "Behaviour.hpp"


class Targetable : public Behaviour {

public:

    // ------------------------------
    // | Constructors
    // ------------------------------

    Targetable();


    // ------------------------------
    // | Public methods
    // ------------------------------

    /// <summary>
    /// Registers a new targeting, therefore incrementing the targeting counter.
    /// </summary>
    /// <returns>A boolean value denoting whether the targeting was successful.</returns>
    bool target();

    /// <summary>
    /// Checks if the entity is targetable.
    /// </summary>
    /// <returns>A boolean value denoting whether the entity is targetable.</returns>
    bool canTarget();

    /// <summary>
    /// Decrements the targeting counter.
    /// </summary>
    /// <returns>A boolean value denoting whether the removal of targeting was succesful.</returns>
    bool removeTargeting();


    // ------------------------------
    // | Overrides
    // ------------------------------

    // ..


    // ------------------------------
    // | Getters/setters
    // ------------------------------

    int getMaxTargetingCount() const { return maxTargetingCount; }
    void setMaxTargetingCount(int value) { maxTargetingCount = value; }

    void setTargetable(bool value) { targetable = value; }

    int getTargetingCount() const { return targetingCount; }


private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    // ..


    // ------------------------------
    // | Fields
    // ------------------------------

    int maxTargetingCount;
    int targetingCount;
    bool targetable;

};

