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
#include "../Targetable.hpp"
#include "../GameManager.hpp"

class Poacher : public Behaviour
{
	friend class PoacherIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;
public:

	enum class State
	{
		Hunting,
		Scouting,
		Shooting,
		GettingTrophy,
		Leaving
	};


	// ------------------------------
	// | Constructors
	// ------------------------------

	Poacher(const Vec3& startingPos);

	// ------------------------------
	// | Overrides
	// ------------------------------

	void start(Entity& entity) override;
	void update(Entity& entity) override;

	// ------------------------------
	// | Getters/setters
	// ------------------------------

	int getIdleCounter() const { return idleCounter; }
	void setIdleCounter(int value) { idleCounter = value; }

	std::string getPoacherlabel() { return poacherLabel; }
	void setPoacherLabel(std::string value) { poacherLabel = value; }

	std::string getCarcassName() { return carcassName; }
	void setCarcassName(std::string value) { carcassName = value; }

	int getLocalId() { return localId; }
	void setLocalId(int value) { localId = value; }

	Vec3 getStartingPos() { return startingPos; }

	State getState() { return state; }
	void setState(State value) { state = value; }

	bool getSpotted() { return spotted; }
	void setSpotted(bool value) { spotted = value; }

	// ------------------------------
	// | Public methods
	// ------------------------------

	void setUniqueId(int id);

private:

	// ------------------------------
	// | Private methods
	// ------------------------------

	Entity* getClosestAnimal();
	//Entity* getClosestCarcass();
	void enterScouting();
	void enterHunting();
	void enterShooting();
	void enterGettingTrophy();
	void enterLeaving();
	void setTrophy(Entity& newTrophy);
	void rotate(Camera& camera, Transform& trans);
	void die();

	// ------------------------------
	// | Fields
	// ------------------------------

	int localId; //maybe, idk yet, if we will have groups of poachers, its kinda something that I'd like to do
	std::string poacherLabel;
	std::string preyName;
	std::string carcassName;

	//int idleMax;
	int idleCounter;

	Entity* prey;
	Entity* trophy;

	const Vec3 startingPos;

	State state;

	bool spotted;

};
