#pragma once

#include "Behaviour.hpp"
#include "Camera.hpp"
#include "Behaviour.hpp"
#include "Input.hpp"
#include "Transform.hpp"
#include "Labels.hpp"
#include "GridSelector.hpp"
#include "Grid.hpp"
#include "Texture.hpp"
#include "UIBehaviour.hpp"
#include "Road.hpp"
#include <unordered_set>
#include "./Animals/Zebra.hpp"
#include "./Animals/Wildebeest.hpp"
#include "./Animals/Giraffe.hpp"
#include "./Animals/Lion.hpp"
#include "GameManager.hpp"

enum State{
    travelingForward,
    travelingBack,
    waitingToScore,
};

class Jeep: public Behaviour {

    friend class JeepIntegrationTest_JeepReachesCheckpointAndUpdatesRoadIndex_Test;
    friend class JeepIntegrationTest_JeepReachesEndOfPathAndChangesState_Test;
    friend class JeepIntegrationTest_JeepRotatesTowardCameraAndDestination_Test;
    friend class JeepIntegrationTest_RotateFacesCameraAndFlipsWhenDestinationIsBehind_Test;

protected:

	void start(Entity& self) override;
	void update(Entity& self) override;

    void Move(); //no camelcases???
    void checkForAnimal();

private:

    float checkArrivedInterval = 2.f;
    float checkArrivedTimer = 0.f;

    float score;

    std::unordered_set<std::string> seenAnimals;
    std::unordered_set<std::string> seenTypes;

    int currentRoadIndex;
	std::vector<Ivec2> roadList;

	GridSelector& gs;
	Grid& grid;
	UIBehaviour& uib;
    Road& road;
    Camera &camera;

    void rotate(Camera &camera, Transform &trans);

public:
    void setUniqueId(int id);

    void AtLocation(Ivec2 loc, Entity &ent);

    void Score();

    State state;

    std::string jeepLabel;
    Vec3 pos;

    Entity* global;

	// Constructor with initializer list
	Jeep(Vec3 pos)
		: gs(Labels::getUnique("global").getComponent<GridSelector>()),
		grid(Labels::getUnique("global").getComponent<Grid>()),
		uib(Labels::getUnique("global").getComponent<UIBehaviour>()),
        road(Labels::getUnique("global").getComponent<Road>()),
        camera(Labels::getUnique("global").getComponent<Camera>())
	{
        this->pos = pos;
        state = travelingForward;
        score = 0.0f;
        global = &Labels::getUnique("global");
	}
};