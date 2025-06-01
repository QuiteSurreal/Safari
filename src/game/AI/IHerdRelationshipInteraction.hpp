#pragma once

#include "Utils.hpp"

class IHerdRelationshipInteraction {

public:

    /// <summary>
    /// Calculates how many animals from the herd are inside the territory of the caller herd.
    /// </summary>
    /// <param name="otherHerdCenter">The other herd entity's position.</param>
    /// <param name="otherHerdTerritoryRadius">The other herd entity's territory's radius.</param>
    /// <returns></returns>
    virtual int getIntersectingAnimals(Vec3 otherHerdCenter, float otherHerdTerritoryRadius) = 0;
    
};

