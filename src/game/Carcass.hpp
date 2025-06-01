#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <random>
#include "Behaviour.hpp"
#include "Labels.hpp"
#include "Health.hpp"
#include "Time.hpp"
#include "Camera.hpp"
#include "DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "Targetable.hpp"

class Carcass : public Behaviour
{
public:

	enum class State
	{
		Dead
	};

	// ------------------------------
	// | Constructors
	// ------------------------------

	Carcass();

	// ------------------------------
	// | Overrides
	// ------------------------------

	void start(Entity& entity) override;
	void update(Entity& entity) override;

	// ------------------------------
	// | Getters/setters
	// ------------------------------

	int getAge() { return age; }
	void setAge(int value) { age = value; }

	Uint getLastHour() { return lastHour; }
	void setLastHour(Uint value) { lastHour = value; }

	int getRemainingCalories() { return remainingCalories; }

	bool getDeathSpiral() { return deathSpiral; }
	void setDeathSpiral(bool value) { deathSpiral = value; }


	// ------------------------------
	// | Public methods
	// ------------------------------

	void getDragged(Vec3 dest, float speed);

private:

	// ------------------------------
	// | Private methods
	// ------------------------------

	void rot();
	void die();
	void rotate(Camera& camera, Transform& trans);

	// ------------------------------
	// | Fields
	// ------------------------------

	int age;
	Uint lastHour;
	int remainingCalories;
	//this can be set off by others
	bool deathSpiral;

};
