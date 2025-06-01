#include "GameManager.hpp"

GameManager::GameManager()
    : ui {Labels::getUnique("global").getComponent<UIBehaviour>()}
{
    money = 0;
    jeepCount = 0;
    enRouteCount = 0;
    rating = 1.0f;
    waitingVisitors = 0;
    difficulty = Difficulty::Easy;
    lastHour = 0;
    animalCount = 0;
    gameStarted = false;
    gameEnded = false;

    totalRating = 0.0f;
    numRating = 0;
}

void GameManager::start(Entity&)
{
}

void GameManager::update(Entity& entity)
{

    if (ui.clickGame) {


        Seeder::resetWorld();
        entity.getComponent<Road>().Reset();
        reset();
        /*if (Labels::isLabelUsed("eco"))
        {
            logDebug("whuh");
            Labels::getUnique("eco").destroy();
        }*/
        Seeder::seedMain();
        

        if (ui.difficulty == "easy")
        {
            difficulty = Difficulty::Easy;
        }
        else if (ui.difficulty == "normal")
        {
            difficulty = Difficulty::Normal;
        }
        else
        {
            difficulty = Difficulty::Hard;
        }

        
        entity.getComponent<Camera>().enable();
        entity.getComponent<Time>().reset();


        running = true;

        money = 4000;

        changeUI();
    }
    /*else if (!running)
    {
        return;
    }*/

    if (ui.clickMenu) {
        running = false;

        /*Seeder::resetWorld();
        entity.getComponent<Road>().Reset();
        reset();
        if (Labels::isLabelUsed("eco"))
        {
            logDebug("whuh");
            Labels::getUnique("eco").destroy();
        }
        Seeder::seedMenu();*/

        entity.getComponent<Camera>().disable();
    }

    if (running && money > 10000 && rating > 4.0f)
    {
        ui.openResult(true);
        running = false;
        entity.getComponent<Camera>().disable();
    }
    else if (running && animalCount <= 0)
    {
        ui.openResult(false);
        running = false;
        entity.getComponent<Camera>().disable();
    }



    Uint currHour = Labels::getUnique("global").getComponent<Time>().getHours();
	if (currHour != lastHour)
	{
		float rand = randf(0, 1);
        if (rand / (1/rating) > 0.8f)
        {
            waitingVisitors++;
            ui.setCount(waitingVisitors, "waitingVisitorsCount");
        }
		lastHour = currHour;
	}

    Road* road = &Labels::getUnique("global").getComponent<Road>();

    if (waitingVisitors >= 5 && jeepCount >= 1 && road->GetRoadList().size() > 0)
    {
        Ivec2 startPos = road->getEntryPoint();
        removeJeep();
        addEnRoute();
        waitingVisitors -= 5;
        ui.setCount(waitingVisitors, "waitingVisitorsCount");
        Factory::createJeep(Vec3(startPos.x-25+0.5, 0, startPos.y-25));
        addMoney(500);
    }
}

//give back a global speed effect
float GameManager::getSpeed()
{
	Time::Speed speed = Labels::getUnique("global").getComponent<Time>().getSpeed();

	if (speed == Time::Speed::Hours) return 1.0f;
	else if (speed == Time::Speed::Days) return 2.0f;
	else return 5.0f;
}

//add a new rating and calculate the mean
void GameManager::addRating(float newRate)
{
    totalRating += newRate;
    numRating++;
    rating = totalRating/numRating;
    ui.setRating(rating);
}

//reset all variables when the game is started
void GameManager::reset()
{
    money = 0;
    jeepCount = 0;
    enRouteCount = 0;
    rating = 1.0f;
    waitingVisitors = 0;
    difficulty = Difficulty::Easy;
    lastHour = 0;
    animalCount = 0;
    running = false;
    gameEnded = false;

    totalRating = 0.0f;
    numRating = 0;
}

//remove money, if you have enough, and update the corresponding ui element
bool GameManager::buyItem(GameManager::Items item)
{
    int currMoney = money;

    switch (item)
    {
        case Items::Balloon:          money -= static_cast<int>(balloonP * currMult()); break;
        case Items::Camera:           money -= static_cast<int>(cameraP * currMult()); break;
        case Items::DroneStation:     money -= static_cast<int>(droneStationP * currMult()); break;
        case Items::Hunter:           money -= static_cast<int>(hunterP * currMult()); break;
        case Items::HunterHut:        money -= static_cast<int>(hunterHutP * currMult()); break;
        case Items::Jeep:             money -= static_cast<int>(jeepP * currMult()); break;
        case Items::Tracker:          money -= static_cast<int>(trackerP * currMult()); break;
        case Items::CanePrice:        money -= static_cast<int>(caneP * currMult()); break;
        case Items::NormalGrassPrice: money -= static_cast<int>(normalGrassP * currMult()); break;
        case Items::RedGrassPrice:    money -= static_cast<int>(redGrassP * currMult()); break;
        case Items::ExoticGrassPrice: money -= static_cast<int>(exoticGrassP * currMult()); break;
        default:
            throw std::runtime_error("Unknown game item.");
    }

    if (money < 0)
    {
        money = currMoney;
        return false;
    }
    else
    {
        ui.setMoney(money);
        return true;
    }
}

//in the event of a game start, update all prices
void GameManager::changeUI()
{
    switch(difficulty)
    {
        case Difficulty::Easy:
            ui.setPrice(hunterHutP*0.8, "hunterHutPrice");
            ui.setPrice(balloonP*0.8, "balloonPrice");
            ui.setPrice(cameraP*0.8, "cameraPrice");
            ui.setPrice(hunterHutP*0.8, "hunterPrice");
            ui.setPrice(droneStationP*0.8, "droneStationPrice");
            ui.setPrice(caneP*0.8, "canePrice");
            ui.setPrice(normalGrassP*0.8, "normalGrassPrice");
            ui.setPrice(redGrassP*0.8, "redGrassPrice");
            ui.setPrice(exoticGrassP*0.8, "exoticGrassPrice");
            ui.setPrice(trackerP*0.8, "trackerPrice");
            ui.setPrice(jeepP*0.8, "jeepPrice");
            ui.setMoney(money);
            break;
        case Difficulty::Normal:
            ui.setPrice(hunterHutP, "hunterHutPrice");
            ui.setPrice(balloonP, "balloonPrice");
            ui.setPrice(cameraP, "cameraPrice");
            ui.setPrice(hunterHutP, "hunterPrice");
            ui.setPrice(droneStationP, "droneStationPrice");
            ui.setPrice(caneP, "canePrice");
            ui.setPrice(normalGrassP, "normalGrassPrice");
            ui.setPrice(redGrassP, "redGrassPrice");
            ui.setPrice(exoticGrassP, "exoticGrassPrice");
            ui.setPrice(trackerP, "trackerPrice");
            ui.setPrice(jeepP, "jeepPrice");
            money = 3000;
            ui.setMoney(3000);
            break;
        case Difficulty::Hard:
            ui.setPrice(hunterHutP*1.2, "hunterHutPrice");
            ui.setPrice(balloonP*1.2, "balloonPrice");
            ui.setPrice(cameraP*1.2, "cameraPrice");
            ui.setPrice(hunterP*1.2, "hunterPrice");
            ui.setPrice(droneStationP*1.2, "droneStationPrice");
            ui.setPrice(caneP*1.2, "canePrice");
            ui.setPrice(normalGrassP*0.8, "normalGrassPrice");
            ui.setPrice(redGrassP*1.2, "redGrassPrice");
            ui.setPrice(exoticGrassP*1.2, "exoticGrassPrice");
            ui.setPrice(trackerP*1.2, "trackerPrice");
            ui.setPrice(jeepP*1.2, "jeepPrice");
            money = 2000;
            ui.setMoney(2000);
            break;
        default:
	        throw std::runtime_error("Unknown game state.");
	        break;
    }
}