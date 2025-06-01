#include <gtest/gtest.h>
#include "Jeep.hpp"
#include "../framework/Transform.hpp"
#include "../framework/Entity.hpp"
#include "../framework/Labels.hpp"
#include "../game/GameManager.hpp"
#include "../game/DestinationBasedMovement/DestinationBasedMovement.hpp"


TEST(JeepIntegrationTest, JeepIsCreatedAndStartsAtEntryPoint) {
    Vec3 entryPoint(25, 0, 25);
    Entity* jeepEntity = &Factory::createJeep(entryPoint);

    ASSERT_NE(jeepEntity, nullptr);
    ASSERT_TRUE(jeepEntity->hasComponent<Jeep>());
    ASSERT_TRUE(jeepEntity->hasComponent<Transform>());

    const auto& jeep = jeepEntity->getComponent<Jeep>();
    const auto& transform = jeepEntity->getComponent<Transform>();

    Vec3 actualPos = transform.getPosition();
    EXPECT_NEAR(actualPos.x, entryPoint.x, 0.01);
    EXPECT_NEAR(actualPos.z, entryPoint.z, 0.01);
}

TEST(JeepIntegrationTest, JeepHasCorrectLabelAfterSettingID) {
    Entity* jeepEntity = &Factory::createJeep({0, 0, 0});
    ASSERT_NE(jeepEntity, nullptr);
    
    auto& jeep = jeepEntity->getComponent<Jeep>();
    jeep.setUniqueId(42);

    ASSERT_TRUE(jeepEntity->hasComponent<Labels>());
}

TEST(JeepIntegrationTest, JeepReachesCheckpointAndUpdatesRoadIndex) {
    Entity* jeepEntity = &Factory::createJeep({0, 0, 0});
    ASSERT_NE(jeepEntity, nullptr);

    auto& jeep = jeepEntity->getComponent<Jeep>();

    jeep.currentRoadIndex = 0;

    jeep.roadList.push_back(Ivec2(0,0));
    jeep.roadList.push_back(Ivec2(0,1));
    jeep.roadList.push_back(Ivec2(0,2));
    jeep.roadList.push_back(Ivec2(0,3));

    Ivec2 nextPoint = jeep.roadList[1];
    jeep.pos.x = 0;
    jeep.pos.z = 1;

    jeep.AtLocation(nextPoint, *jeepEntity);
    
    EXPECT_GE(jeep.currentRoadIndex, 1);
}


TEST(JeepIntegrationTest, JeepReachesEndOfPathAndChangesState) {
    //Factory::createGlobal();
    Entity* jeepEntity = &Factory::createJeep({0, 0, 0});
    ASSERT_NE(jeepEntity, nullptr);

    auto& jeep = jeepEntity->getComponent<Jeep>();

    jeep.roadList.push_back(Ivec2(0,0));
    jeep.roadList.push_back(Ivec2(0,1));
    jeep.roadList.push_back(Ivec2(0,2));
    jeep.roadList.push_back(Ivec2(0,3));

    jeep.currentRoadIndex = jeep.roadList.size() - 1;
    jeep.state = travelingForward;

    Ivec2 currentLoc = jeep.roadList[jeep.currentRoadIndex];
    jeep.pos = Vec3(currentLoc.x, 0, currentLoc.y);

    jeep.AtLocation(currentLoc, *jeepEntity);

    EXPECT_EQ(jeep.state, waitingToScore);
}



TEST(JeepIntegrationTest, RotateFacesCameraAndFlipsWhenDestinationIsBehind) {
    using namespace std;

    Vec3 spawnPos(0, 0, 0);
    Entity* jeepEntity = &Factory::createJeep(spawnPos);
    ASSERT_NE(jeepEntity, nullptr);
    ASSERT_TRUE(jeepEntity->hasComponent<Jeep>());
    ASSERT_TRUE(jeepEntity->hasComponent<Transform>());

    Jeep& jeep = jeepEntity->getComponent<Jeep>();
    Transform& trans = jeepEntity->getComponent<Transform>();

    Camera& cam = Labels::getUnique("global").addComponent<Camera>();
    cam.center = Vec3(0,0,5);
    cam.calcViewPos();
    cam.rot = Vec3(0, 0, -1);

    jeep.rotate(cam, trans);
    float angleFacingCam = trans.getLocalRotation().y;
    EXPECT_NEAR(angleFacingCam, 0.0f, 0.01f);

    jeepEntity->addComponent<DestinationBasedMovement>().setDestination(Vec3(0, 0, 10));
    jeep.rotate(cam, trans);
    float angleFlipped = trans.getLocalRotation().y;
    EXPECT_NEAR(angleFlipped, 0.15f, 0.5f);

    cam.center = Vec3(0,0,5);
    cam.calcViewPos();
    cam.rot = Vec3(-1, 0, 0);
    jeep.rotate(cam, trans);
    float angleRight = trans.getLocalRotation().y;
    EXPECT_NEAR(angleRight, -0.15f / 2, 0.5f);
}
