#include "Targetable.hpp"

Targetable::Targetable() {
    maxTargetingCount = std::numeric_limits<int>::max();
    targetingCount = 0;
    targetable = true;
}

bool Targetable::target() {
    if (canTarget()) {
        targetingCount++;
        return true;
    }
    else {
        return false;
    }
}

bool Targetable::canTarget() {
    return targetable && targetingCount < maxTargetingCount;
}

bool Targetable::removeTargeting() {
    if (targetingCount > 0) {
        targetingCount--;
        return true;
    }
    else {
        return false;
    }
}
