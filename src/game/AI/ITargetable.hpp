#pragma once

#include "Utils.hpp"
#include "Component.hpp"

class ITargetable {

public:

    /// <summary>
    /// Registers a new targeting entity.
    /// </summary>
    /// <returns>A boolean value denoting whether the targeting was successful.</returns>
    virtual bool target(Entity* ent) = 0;

    /// <summary>
    /// Checks if the targeted entity is alive. If the entity is dead, the targeting entity is unregistered.
    /// </summary>
    /// <returns>A boolean value denoting whether the targeted entity is alive.</returns>
    virtual bool checkHeartbeat(Entity* ent) = 0;

    ///just kill me
    
};

