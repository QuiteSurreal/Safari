#pragma once

#include <optional>

#include "Behaviour.hpp"
#include "tuple"
#include "Input.hpp"
#include "../Camera.hpp"
#include "Labels.hpp"
#include "../Factory.hpp"
#include "../DestinationBasedMovement/DestinationBasedMovement.hpp"

class DroneStation : public Behaviour
{
	friend class DroneStationIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
public:

	DroneStation(Vec3 position);

	void start(Entity& entity) override;
	void update(Entity& entity) override;

	void setUniqueId(int id);

private:

	void rotate(Camera& camera, Transform& trans);


	std::string stationLabel;
	Vec3 pos;
};