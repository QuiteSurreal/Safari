#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include "Behaviour.hpp"
#include "Labels.hpp"
#include "Health.hpp"
#include "../Animals/Zebra.hpp"
#include "../Time.hpp"
#include "../Carcass.hpp"
#include "../Camera.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "Poacher.hpp"
#include "../Targetable.hpp"

class Hunter : public Behaviour
{
	friend class HunterIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
public:

	enum class State
	{
        Idling,
		Travelling,
        Hunting,
        Shooting
	};


	// ------------------------------
	// | Constructors
	// ------------------------------

	Hunter(const Vec3 startingPos);

	// ------------------------------
	// | Overrides
	// ------------------------------

	void start(Entity& entity) override;
	void update(Entity& entity) override;

	// ------------------------------
	// | Getters/setters
	// ------------------------------

	std::string getHunterlabel() { return hunterLabel; }
	void setHunterLabel(std::string value) { hunterLabel = value; }

	int getLocalId() { return localId; }
	void setLocalId(int value) { localId = value; }

	Vec3 getStartingPos() { return startingPos; }
    void setStartingPos(Vec3 value) { startingPos = value; }

    int getHutId() { return hutId; }
    void setHutId(int value) { hutId = value; }

	State getState() { return state; }
	void setState(State value) { state = value; }

	void enterTravelling(Vec3 point);
	void enterHunting(Entity* t);


	// ------------------------------
	// | Public methods
	// ------------------------------

	void setUniqueId(int id);

private:

	// ------------------------------
	// | Private methods
	// ------------------------------

	void enterIdling();
	void enterShooting();
	void setTarget(Entity& newTarget);
	void rotate(Camera& camera, Transform& trans);
	Entity* checkForSpotted();

	// ------------------------------
	// | Fields
	// ------------------------------

	int localId;
	std::string hunterLabel;
    int hutId;

	Entity* target;

    Vec3 startingPos;
	Vec3 patrolPos;

	State state;
	int idleCounter;
	int firerate;
};
