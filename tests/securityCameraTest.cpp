#include <gtest/gtest.h>
#include "../game/InteractableObjects/Drone.hpp"
#include "../framework/Entity.hpp"
#include "../framework/Labels.hpp"

TEST(SecurityCameraIntegrationTest, RotateFacesCameraAndFlipsWhenDestinationIsBehind) {
    using namespace std;

    Vec3 spawnPos(0, 0, 0);
    Entity* objEntity = &Factory::createSecurityCamera(spawnPos);
    ASSERT_NE(objEntity, nullptr);
    ASSERT_TRUE(objEntity->hasComponent<SecurityCamera>());
    ASSERT_TRUE(objEntity->hasComponent<Transform>());

    SecurityCamera& ent = objEntity->getComponent<SecurityCamera>();
    Transform& trans = objEntity->getComponent<Transform>();

    Camera& cam = Labels::getUnique("global").addComponent<Camera>();
    cam.center = Vec3(0,0,5);
    cam.calcViewPos();
    cam.rot = Vec3(0, 0, -1);

    ent.rotate(cam, trans);
    float angleFacingCam = trans.getLocalRotation().y;
    EXPECT_NEAR(angleFacingCam, -0.5, 0.5f);

}
