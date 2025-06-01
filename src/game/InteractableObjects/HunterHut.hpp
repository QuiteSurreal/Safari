#pragma once

#include <optional>

#include "Behaviour.hpp"
#include "tuple"
#include "Input.hpp"
#include "../Camera.hpp"
#include "Labels.hpp"
#include "../Factory.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "Hunter.hpp"

class HunterHut : public Behaviour
{
    friend class HunterHutIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
public:

	HunterHut(Vec3 position);

	void start(Entity& entity) override;
	void update(Entity& entity) override;

	void setUniqueId(int id);

    int getHutSize() { return hutSize; }
    void setHutSize(int value) { hutSize = value; }

    bool getDeployHunter() { return deployHunter; }
    void setDeployHunter(bool value) { deployHunter = value; }

    Vec3 getPos() { return pos; }

    const std::tuple<int, int, std::string> addHunter();
    void removeHunterFromHut(int localId);

private:

	void rotate(Camera& camera, Transform& trans);

    int globalHutId;
    int hutSize;
	std::string hutLabel;
	Vec3 pos;
    bool deployHunter;
};