#pragma once

#include "Utils.hpp"
#include "Behaviour.hpp"
#include "Labels.hpp"
#include "../game/Camera.hpp"
#include "Input.hpp"
#include "../game/DestinationBasedMovement/DestinationBasedMovement.hpp"

class AoECow : public Behaviour {

public:

	void update(Entity& entity) override;

};
