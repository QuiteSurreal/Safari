#pragma once

#include "Utils.hpp"
#include <vector>

class IDestinationBasedMovementStrategy {

public:

    virtual std::vector<Vec3> buildPath(Vec3& destination) = 0;

};
