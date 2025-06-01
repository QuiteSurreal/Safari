#pragma once
#include "Behaviour.hpp"
#include "UIBehaviour.hpp"
#include "Labels.hpp"
#include "Time.hpp"
#include "Road.hpp"
#include "Seeder.hpp"


class GameManager : public Behaviour
{
public:

	enum class Difficulty
	{
        Easy,
		Normal,
        Hard
	};

	enum class Items
	{
		HunterHut,
		Balloon,
		Jeep,
		Camera,
		DroneStation,
		Hunter,
		Tracker,
		CanePrice,
		NormalGrassPrice,
		RedGrassPrice,
		ExoticGrassPrice,
	};


	// ------------------------------
	// | Constructors
	// ------------------------------

    GameManager();

	// ------------------------------
	// | Overrides
	// ------------------------------

	void start(Entity& entity) override;
	void update(Entity& entity) override;

	// ------------------------------
	// | Getters/setters
	// ------------------------------

	//Difficulty getDifficulty() { return difficulty; }
	//void setDifficulty(Difficulty value) { difficulty = value; diffChange = true; }

    int getJeepCount() { return jeepCount; }
	void setJeepCount(int value) { jeepCount = value; ui.setCount(jeepCount, "jeepCount"); }

    int getWaitingVisitors() { return waitingVisitors; }
	void setWaitingVisitors(int value) { waitingVisitors = value; }

	bool getGameStarted() { return gameStarted; }
	void setGameStarted(bool value) { gameStarted = value; }

	bool getGameEnded() { return gameEnded; }
	void setGameEnded(bool value) { gameEnded = value; }


	// ------------------------------
	// | Public methods
	// ------------------------------

	void addJeep() { jeepCount++ ; ui.setCount(jeepCount, "jeepCount"); }
	void removeJeep() { jeepCount-- ; ui.setCount(jeepCount, "jeepCount"); }
	void addEnRoute() { enRouteCount++ ; ui.setCount(enRouteCount, "enRouteCount"); }
	void removeEnRoute() { enRouteCount-- ; ui.setCount(enRouteCount, "enRouteCount"); }
	void addMoney(int value) { money += value; ui.setMoney(money); }
	void addAnimal() { animalCount++; }
	void removeAnimal() { animalCount--; }
	void addRating(float newRate);

	void reset();
	
	float currMult() { if (difficulty == Difficulty::Easy) {return 0.8f;} else if (difficulty == Difficulty::Normal) {return 1.0f;} else {return 1.2f;}}
	bool buyItem(GameManager::Items item);
	float getSpeed();

private:

	// ------------------------------
	// | Private methods
	// ------------------------------

	bool running = false;

	void changeUI();

	// ------------------------------
	// | Fields
	// ------------------------------

	int money;
    int jeepCount;
	int enRouteCount;
	float rating;
	float totalRating;
	int numRating;
    int waitingVisitors;
	int waittime = 10;
	bool diffChange;
	int animalCount;

	bool gameStarted;
	bool gameEnded;

	Difficulty difficulty;

    UIBehaviour &ui;

	Uint lastHour;

	int hunterHutP = 400;
	int droneStationP = 500;
	int balloonP = 650;
	int hunterP = 800;
	int cameraP = 200;

	int jeepP = 800;
	int trackerP = 200;

	int caneP = 10;
	int normalGrassP = 10;
	int redGrassP = 10;
	int exoticGrassP = 10;
};
