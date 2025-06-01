#pragma once

#include "IDestinationBasedMovementStrategy.hpp"
#include "Utils.hpp"

class LinearDestinationMovementStrategy : public IDestinationBasedMovementStrategy {

public:
   
    std::vector<Vec3> buildPath(Vec3& destination) override;

};

