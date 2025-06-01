#pragma once

#include "Behaviour.hpp"
#include "../Camera.hpp"
#include "Labels.hpp"
#include "../Factory.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "../Animals/Zebra.hpp"
#include "../Animals/ZebraHerd.hpp"
#include "./Poacher.hpp"

class SecurityCamera : public Behaviour
{
	friend class SecurityCameraIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
public:
	SecurityCamera();

	void start(Entity& entity) override;
	void update(Entity& entity) override;

private:

	void rotate(Camera& camera, Transform& trans);

	void checkForSpot();
};