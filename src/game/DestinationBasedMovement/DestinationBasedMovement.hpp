#pragma once

#include <vector>
#include <memory>
#include "Utils.hpp"
#include "Behaviour.hpp"
#include "IDestinationBasedMovementStrategy.hpp"
#include "LinearDestinationBasedMovementStrategy.hpp"
#include "Component.hpp"
#include "Transform.hpp"

class DestinationBasedMovement : public Behaviour {
    friend class DroneIntegrationTest_Update_TransitionsStatesAndAssignsHerd_Test;
public:

    static constexpr float EPSILON = 0.05f;

    // ------------------------------
    // | Constructors
    // ------------------------------

    DestinationBasedMovement();
    DestinationBasedMovement(std::unique_ptr<IDestinationBasedMovementStrategy> strategy);


    // ------------------------------
    // | Public methods
    // ------------------------------

    void updatePath();


    // ------------------------------
    // | Overrides
    // ------------------------------

    void start(Entity& entity) override;
    void update(Entity& entity) override;


    // ------------------------------
    // | Getters/setters
    // ------------------------------

    // strategy
    const IDestinationBasedMovementStrategy& getMovementStrategy() const;
    void setMovementStrategy(std::unique_ptr<IDestinationBasedMovementStrategy> newStrategy);

    // destination
    const Vec3& getDestination() const;
    void setDestination(const Vec3& destination);

    // path
    const std::vector<Vec3>& GetPath() const;

    // speed
    float getSpeed();
    void setSpeed(float speed);

    // arrived
    bool getArrived();


private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    // (...)


    // ------------------------------
    // | Fields
    // ------------------------------

    // --- timers ---
    float accumulator;
    float timeStep;
    // ---

    std::unique_ptr<IDestinationBasedMovementStrategy> strategy;            // The behaviour's path building strategy.
    Vec3 destination;                                                       // The final destination of the movement behaviour.
    std::vector<Vec3> path;                                                 // A path made of vectors on which the entity will move through.
    float speed;                                                            // The speed of the movement.
    bool arrived;


};
