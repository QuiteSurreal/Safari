#include <gtest/gtest.h>
#include "../game/InteractableObjects/Drone.hpp"
#include "../framework/Entity.hpp"
#include "../framework/Labels.hpp"

TEST(HunterIntegrationTest, RotateFacesCameraAndFlipsWhenDestinationIsBehind) {
    using namespace std;

    Vec3 spawnPos(0, 0, 0);
    Entity* objEntity = &Factory::createHunter(spawnPos,0,0,"");
    ASSERT_NE(objEntity, nullptr);
    ASSERT_TRUE(objEntity->hasComponent<Hunter>());
    ASSERT_TRUE(objEntity->hasComponent<Transform>());

    Hunter& ent = objEntity->getComponent<Hunter>();
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
