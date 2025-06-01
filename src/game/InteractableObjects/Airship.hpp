#pragma once

#include "Behaviour.hpp"
#include "../Camera.hpp"
#include "Labels.hpp"
#include "../Factory.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "../Animals/Zebra.hpp"
#include "../Animals/ZebraHerd.hpp"
#include "Poacher.hpp"

class Airship : public Behaviour
{
	friend class AirshipIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
public:

	enum class State
	{
		ChangeMode,
		Moving
	};

	Airship();

	void start(Entity& entity) override;
	void update(Entity& entity) override;

	//void setUniqueId(int id);

	void setMode(int value) { mode = value; state = State::ChangeMode; }

private:

	void rotate(Camera& camera, Transform& trans);

	void checkForSpot();

	int cornerNum;

	State state;

	int mode;

	void enterChangeMode();
	void enterMoving();
};