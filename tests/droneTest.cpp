#include <gtest/gtest.h>
#include "../game/InteractableObjects/Drone.hpp"
#include "../framework/Entity.hpp"
#include "../framework/Labels.hpp"
#include "../game/Animals/ZebraHerd.hpp"
#include "../game/Animals/WildebeestHerd.hpp"
#include "../game/Animals/LionPride.hpp"

TEST(DroneIntegrationTest, AssignHerd_AssignsToUnoccupiedHerd) {
    using namespace std;

    srand(0);

    Entity& ecosystem = Factory::createEcosystem();
	HerdManager& herdManager = ecosystem.getComponent<HerdManager>();
	herdManager.createZebraHerd(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 5);

    Entity droneEntity = Factory::createDrone(Vec3(0,0,0), NULL);
    Drone drone = droneEntity.getComponent<Drone>();
    Entity* assigned = nullptr;

    for (int i = 0; i < 30; ++i)
    {
        assigned = drone.assignHerd();
        if (assigned)
        {
            break;
        }
    }

    ASSERT_NE(assigned, nullptr);
    auto& labels = assigned->getComponent<Labels>();
    EXPECT_TRUE(labels.hasLabel("hasDrone"));
}


TEST(DroneIntegrationTest, CheckForSpot_SpotsAndIgnoresCorrectly) {

    Entity droneEntity = Factory::createDrone(Vec3(0,0,0), NULL);
    Drone drone = droneEntity.getComponent<Drone>();

    Entity inRangePoacher = Factory::createPoacher();
    inRangePoacher.getComponent<Transform>().relocate(Ivec3(0,0,0));

    Entity outOfRangePoacher = Factory::createPoacher();
    outOfRangePoacher.getComponent<Transform>().relocate(Ivec3(20,0,20));

    drone.checkForSpot();

    EXPECT_TRUE(inRangePoacher.getComponent<Poacher>().getSpotted());
    EXPECT_FALSE(outOfRangePoacher.getComponent<Poacher>().getSpotted());

}


TEST(DroneIntegrationTest, StateTransitions_SetCorrectStateAndDestination) {
    Vec3 startPos(0, 0, 0);
    Entity droneEntity = Factory::createDrone(startPos, nullptr);
    Drone& drone = droneEntity.getComponent<Drone>();

    drone.homePos = Vec3(10.0f, 0.0f, 10.0f);

    auto& move = droneEntity.getComponent<DestinationBasedMovement>();

    drone.enterAscending();
    EXPECT_EQ(drone.state, Drone::State::Ascending);
    EXPECT_EQ(move.getDestination(), Vec3(10.0f, 6.0f, 10.0f));

    drone.enterDescending();
    EXPECT_EQ(drone.state, Drone::State::Descending);
    EXPECT_EQ(move.getDestination(), Vec3(10.0f, 0.0f, 10.0f));

    drone.enterPatrolling();
    EXPECT_EQ(drone.state, Drone::State::Patrolling);

    drone.enterReturning();
    EXPECT_EQ(drone.state, Drone::State::Returning);
    EXPECT_EQ(move.getDestination(), Vec3(10.0f, 6.0f, 10.0f));

    drone.enterCharging();
    EXPECT_EQ(drone.state, Drone::State::Charging);
}


TEST(DroneIntegrationTest, Update_TransitionsStatesAndAssignsHerd) {
    using namespace std;

    //Entity& ecosystem = Factory::createEcosystem();
	HerdManager& herdManager = Labels::getUnique("eco").getComponent<HerdManager>();
	herdManager.createZebraHerd(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 5);
    Entity* assigned = nullptr;
    Entity droneEntity = Factory::createDrone(Vec3(0,0,0), nullptr);
    Drone& drone = droneEntity.getComponent<Drone>();
    DestinationBasedMovement& dbm = droneEntity.getComponent<DestinationBasedMovement>();

    for (int i = 0; i < 30; ++i)
    {
        assigned = drone.assignHerd();
        if (assigned)
        {
            break;
        }
    }

    srand(0);

    drone.homePos = Vec3(5, 0, 5);
    drone.state = Drone::State::Charging;
    drone.charge = 99.9f;

    drone.update(droneEntity);
    EXPECT_EQ(drone.state, Drone::State::Charging);

    dbm.arrived = true;
    drone.update(droneEntity);
    EXPECT_EQ(drone.state, Drone::State::Ascending);

    drone.charge = 100.1f;
    drone.update(droneEntity);
    EXPECT_EQ(drone.state, Drone::State::Ascending);

}

TEST(DroneIntegrationTest, RotateFacesCameraAndFlipsWhenDestinationIsBehind) {
    using namespace std;

    Vec3 spawnPos(0, 0, 0);
    Entity* objEntity = &Factory::createDrone(spawnPos, NULL);
    ASSERT_NE(objEntity, nullptr);
    ASSERT_TRUE(objEntity->hasComponent<Drone>());
    ASSERT_TRUE(objEntity->hasComponent<Transform>());

    Drone& ent = objEntity->getComponent<Drone>();
    Transform& trans = objEntity->getComponent<Transform>();

    Camera& cam = Labels::getUnique("global").addComponent<Camera>();
    cam.center = Vec3(0,0,5);
    cam.calcViewPos();
    cam.rot = Vec3(0, 0, -1);

    ent.rotate(cam, trans);
    float angleFacingCam = trans.getLocalRotation().y;
    EXPECT_NEAR(angleFacingCam, 0.0f, 0.01f);

    objEntity->addComponent<DestinationBasedMovement>().setDestination(Vec3(0, 0, 10));
    ent.rotate(cam, trans);
    float angleFlipped = trans.getLocalRotation().y;
    EXPECT_NEAR(angleFlipped, 0.15f, 0.5f);

    cam.center = Vec3(0,0,5);
    cam.calcViewPos();
    cam.rot = Vec3(-1, 0, 0);
    ent.rotate(cam, trans);
    float angleRight = trans.getLocalRotation().y;
    EXPECT_NEAR(angleRight, -0.15f / 2, 0.5f);
}
