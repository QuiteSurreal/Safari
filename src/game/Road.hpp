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

class Road: public Behaviour {

	friend class RoadIntegrationTest_DeleteRoad_RemovesCorrectTile_Test;
	friend class RoadIntegrationTest_PlaceRoad_PlacesCorrectTile_Test;
	friend class RoadIntegrationTest_PlaceWater_PlacesCorrectTile_Test;
	friend class RoadIntegrationTest_DeleteWater_RemovesCorrectTile_Test;
	friend class RoadIntegrationTest_UpdateRoad_UpdatesCorrectTileTexture_Test;
	friend class RoadIntegrationTest_GenerateRiver_PlacesRiverAndBridgesCorrectly_Test;

protected:

	void start(Entity& self) override;
	void update(Entity& self) override;

	void DeleteRoad(int X, int Y);

    void DeleteWater(int X, int Y);

    void GenerateRiver();



	static Texture RandomTerrainTexture();

private:

	bool PlaceRoad(int X, int Y);
    bool PlaceWater(int X, int Y);
    void UpdateRoadNeighbours(int X, int Y);
    void UpdateRoad(int X, int Y);
    void UpdateWaterNeighbours(int X, int Y);
    void UpdateWater(int X, int Y);
    bool IsInBound(int X, int Y);

    //int lastRoadX;
	//int lastRoadY;
	Ivec2 entryPoint;
	Ivec2 exitPoint;

	std::vector<Ivec2> bridgeList;

	GridSelector& gs;
	Grid& grid;
	UIBehaviour& uib;

public:

	bool isConnected;

	void Reset();

	std::vector<Ivec2> GetRoadList();
	Ivec2 getEntryPoint() { return entryPoint; }

	// Constructor with initializer list
	Road()
		: gs(Labels::getUnique("global").getComponent<GridSelector>()),
		grid(Labels::getUnique("global").getComponent<Grid>()),
		uib(Labels::getUnique("global").getComponent<UIBehaviour>())
	{
	}
};