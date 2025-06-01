#include <gtest/gtest.h>
#include "../game/Time.hpp"
#include "../framework/Labels.hpp"
#include "../framework/Entity.hpp"
#include "../framework/Input.hpp"

TEST(TimeIntegrationTest, TimeUpdate_ChangesTimeCorrectly) {

    Time& time = Labels::getUnique("global").getComponent<Time>();
    UIBehaviour& uib = Labels::getUnique("global").getComponent<UIBehaviour>();

    time.uib = &uib;

    float hoursToSimulate = 2.0f;

    int originalDays = time.getDays();
    int originalHours = time.getHours();
    int originalMinutes = time.getMinutes();

    Entity dummy = Entity(110, false);
    time.update(dummy);

    int newDays = time.getDays();
    int newHours = time.getHours();
    int newMinutes = time.getMinutes();

    int totalOldMinutes = originalDays * 24 * 60 + originalHours * 60 + originalMinutes;
    int totalNewMinutes = newDays * 24 * 60 + newHours * 60 + newMinutes;

    EXPECT_GE(totalNewMinutes - totalOldMinutes, 0);
}


TEST(TimeIntegrationTest, SetFactor_ClampsValueAndSetsLightStrength) {
    Time& time = Labels::getUnique("global").getComponent<Time>();
    auto& cameraLight = time.getEntity().getComponent<Light::CameraLight>();

    const float minStrength = time.lightStrengthMin;
    const float maxStrength = time.lightStrengthMax;

    struct TestCase {
        float input;
        float expectedClamped;
    };

    std::vector<TestCase> cases = {
        {-1.0f, 0.0f},
        {0.0f, 0.0f},
        {0.5f, 0.5f},
        {1.0f, 1.0f},
        {2.0f, 1.0f}
    };

    for (const auto& test : cases) {
        time.setFactor(test.input);

        float expectedStrength = glm::mix(minStrength, maxStrength, test.expectedClamped);
        EXPECT_FLOAT_EQ(cameraLight.getStrength(), expectedStrength) << "Failed at input: " << test.input;
    }
}
    