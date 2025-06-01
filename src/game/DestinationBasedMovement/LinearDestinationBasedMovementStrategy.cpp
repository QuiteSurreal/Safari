#include "LinearDestinationBasedMovementStrategy.hpp"

std::vector<Vec3> LinearDestinationMovementStrategy::buildPath(Vec3& destination) {
    std::vector<Vec3> path;
    path.push_back(destination);
    return path;
}
