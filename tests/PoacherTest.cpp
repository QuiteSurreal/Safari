#include <gtest/gtest.h>
#include "../game/InteractableObjects/Drone.hpp"
#include "../framework/Entity.hpp"
#include "../framework/Labels.hpp"

TEST(PoacherIntegrationTest, RotateFacesCameraAndFlipsWhenDestinationIsBehind) {
    using namespace std;

    Vec3 spawnPos(0, 0, 0);
    Entity* objEntity = &Factory::createPoacher();
    ASSERT_NE(objEntity, nullptr);
    ASSERT_TRUE(objEntity->hasComponent<Poacher>());
    ASSERT_TRUE(objEntity->hasComponent<Transform>());

    Poacher& ent = objEntity->getComponent<Poacher>();
    Transform& trans = objEntity->getComponent<Transform>();
    trans.relocate(Vec3(0,0,0));

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
    EXPECT_NEAR(angleFlipped, 0.5f, 0.5f);

    cam.center = Vec3(0,0,5);
    cam.calcViewPos();
    cam.rot = Vec3(-1, 0, 0);
    ent.rotate(cam, trans);
    float angleRight = trans.getLocalRotation().y;
    EXPECT_NEAR(angleRight, -0.5f / 2, 0.5f);
}
