#include "DestinationBasedMovement.hpp"
#include "Input.hpp"

DestinationBasedMovement::DestinationBasedMovement() : Behaviour() {
    accumulator = 0.0f;
    timeStep = 1.0f / 60.0f;
    strategy = std::make_unique<LinearDestinationMovementStrategy>();
    destination = Vec3(0.0f, 0.0f, 0.0f);
    speed = 0.0f;
    arrived = false;
}

DestinationBasedMovement::DestinationBasedMovement(std::unique_ptr<IDestinationBasedMovementStrategy> strategy) : Behaviour() {
    accumulator = 0.0f;
    timeStep = 1.0f / 60.0f;
    this->strategy = std::move(strategy);
    destination = Vec3(0.0f, 0.0f, 0.0f);
    speed = 0.0f;
    arrived = false;
}

void DestinationBasedMovement::updatePath() {
    path = strategy->buildPath(destination);
    arrived = false;
}

const IDestinationBasedMovementStrategy& DestinationBasedMovement::getMovementStrategy() const {
    return *strategy;
}

void DestinationBasedMovement::setMovementStrategy (std::unique_ptr<IDestinationBasedMovementStrategy> newStrategy) {
    this->strategy = std::move(newStrategy);
}

const Vec3& DestinationBasedMovement::getDestination() const {
    return destination;
}

void DestinationBasedMovement::setDestination(const Vec3& destination) {
    this->destination = destination;
}

const std::vector<Vec3>& DestinationBasedMovement::GetPath() const {
    return this->path;
}

float DestinationBasedMovement::getSpeed() {
    return this->speed;
}

void DestinationBasedMovement::setSpeed(float speed) {
    this->speed = speed;
}

bool DestinationBasedMovement::getArrived() {
    return arrived;
}

void DestinationBasedMovement::start(Entity& entity) {

}

void DestinationBasedMovement::update(Entity& entity) {

    accumulator += Input::getDeltaTime();

    while (accumulator >= timeStep) {

        // <------ << Timers >> ------>

        accumulator -= timeStep;

        // >>--------<<

        if (path.empty()) {
            arrived = true;
            continue;
        }

        if (glm::distance(entity.getComponent<Transform>().getPosition(), path.front()) <= 0.01f) {
            path.erase(path.begin());
        }
        else {

            Vec3 moveVec = path.front() - entity.getComponent<Transform>().getPosition();
            float distance = glm::length(moveVec);
            if (distance < speed) {
                moveVec = glm::normalize(moveVec) * distance;
            }
            else {
                moveVec = glm::normalize(moveVec) * speed;
            }

            if (glm::length(moveVec) != 0) {
                //logDebug("I am moving");
                entity.getComponent<Transform>().translate(moveVec);
            }

        }

    }
   
}
