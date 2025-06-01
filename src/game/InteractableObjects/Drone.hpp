#pragma once

#include "Behaviour.hpp"
#include "../Camera.hpp"
#include "Labels.hpp"
#include "../Factory.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "../Animals/Zebra.hpp"
#include "../Animals/ZebraHerd.hpp"
#include "../Animals/Wildebeest.hpp"
#include "../Animals/WildebeestHerd.hpp"
#include "../Animals/Giraffe.hpp"
#include "../Animals/GiraffeHerd.hpp"
#include "../Animals/Lion.hpp"
#include "../Animals/LionPride.hpp"
#include "Poacher.hpp"
#include "../AI/HerbivoreHerdAI.hpp"

class Drone : public Behaviour
{
public:

	friend class DroneIntegrationTest_AssignHerd_AssignsToUnoccupiedHerd_Test;
	friend class DroneIntegrationTest_CheckForSpot_SpotsAndIgnoresCorrectly_Test;
	friend class DroneIntegrationTest_StateTransitions_SetCorrectStateAndDestination_Test;
	friend class DroneIntegrationTest_Update_TransitionsStatesAndAssignsHerd_Test;
	friend class DroneIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;

	enum class State
	{
		Patrolling,
		Returning,
		Ascending,
		Descending,
		Charging
	};

	Drone(Vec3 position);

	void start(Entity& entity) override;
	void update(Entity& entity) override;

	//void setUniqueId(int id);

	void setParent(Entity* value) { parentStation = value; }

	void setHerd(Entity* value) { assignedHerd = value; }


private:

	void rotate(Camera& camera, Transform& trans);
	Entity* assignHerd();

	State state;
	float charge;

	int doomClock;

	//std::string stationLabel;
	Vec3 homePos;
	std::string assignedHerdLabel;
	Entity* assignedHerd;
	Entity* parentStation;


	void enterAscending();
	void enterDescending();
	void enterPatrolling();
	void enterReturning();
	void enterCharging();

	void checkForSpot();

};