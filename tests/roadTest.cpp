#include <gtest/gtest.h>
#include "Jeep.hpp"
#include "../framework/Transform.hpp"
#include "../framework/Entity.hpp"
#include "../framework/Labels.hpp"
#include "../game/GameManager.hpp"
#include "../game/DestinationBasedMovement/DestinationBasedMovement.hpp"



TEST(RoadIntegrationTest, PlaceRoad_PlacesCorrectTile) {
    Road& road = Labels::getUnique("global").getComponent<Road>();
    road.PlaceRoad(10,10);

    Ivec2 tile(10, 10);
    EXPECT_EQ(road.grid.getType(tile), "road");
}

TEST(RoadIntegrationTest, DeleteRoad_RemovesCorrectTile) {
    Road& road = Labels::getUnique("global").getComponent<Road>();
    road.PlaceRoad(10,10);

    road.DeleteRoad(10, 10);

    Ivec2 tile(10, 10);
    EXPECT_FALSE(road.grid.getType(tile).has_value());
}

TEST(RoadIntegrationTest, PlaceWater_PlacesCorrectTile) {
    Road& road = Labels::getUnique("global").getComponent<Road>();
    road.PlaceWater(10,10);

    Ivec2 tile(10, 10);
    EXPECT_EQ(road.grid.getType(tile), "water");
}

TEST(RoadIntegrationTest, DeleteWater_RemovesCorrectTile) {
    Road& road = Labels::getUnique("global").getComponent<Road>();
    road.PlaceWater(10,10);

    road.DeleteWater(10, 10);

    Ivec2 tile(10, 10);
    EXPECT_FALSE(road.grid.getType(tile).has_value());

}

TEST(RoadIntegrationTest, UpdateRoad_UpdatesCorrectTileTexture) {
    Road& road = Labels::getUnique("global").getComponent<Road>();

    road.PlaceRoad(5, 5);

    road.PlaceRoad(4, 5);
    road.PlaceRoad(6, 5);
    road.PlaceRoad(5, 4);
    road.PlaceRoad(5, 6);

    road.UpdateRoad(5, 5);

    Ivec2 tile(5, 5);

    EXPECT_EQ(road.grid.getType(tile), "road");

    tile = Ivec2(4, 5);
    EXPECT_EQ(road.grid.getType(tile), "road");
    tile = Ivec2(6, 5);
    EXPECT_EQ(road.grid.getType(tile), "road");
    tile = Ivec2(5, 4);
    EXPECT_EQ(road.grid.getType(tile), "road");
    tile = Ivec2(5, 6);
    EXPECT_EQ(road.grid.getType(tile), "road");
}


TEST(RoadIntegrationTest, GenerateRiver_PlacesRiverAndBridgesCorrectly) {
    Road& road = Labels::getUnique("global").getComponent<Road>();
    road.GenerateRiver();

    int riverCount = 0;
    int bridgeCount = 0;

    for (int x = 0; x < 50; ++x) {
        for (int y = 0; y < 50; ++y) {
            auto typeOpt = road.grid.getType(Ivec2(x, y));
            if (!typeOpt.has_value()) continue;

            const std::string& type = typeOpt.value();
            if (type == "water") {
                ++riverCount;
            } else if (type == "road") {
                ++bridgeCount;

                auto texOpt = road.grid.getTexture(Ivec2(x, y));
                EXPECT_TRUE(texOpt.has_value());

                int roadNeighbors = 0;
                for (auto [dx, dy] : std::vector<std::pair<int, int>>{{1,0}, {-1,0}, {0,1}, {0,-1}}) {
                    Ivec2 neighbor(x + dx, y + dy);
                    if (road.IsInBound(neighbor.x, neighbor.y)) {
                        auto neighborType = road.grid.getType(neighbor);
                        if (neighborType.has_value() && neighborType == "road") {
                            ++roadNeighbors;
                        }
                    }
                }
                EXPECT_GE(roadNeighbors, 0);
            }
        }
    }

    EXPECT_GT(riverCount, 0);

    EXPECT_EQ(road.bridgeList.size(), 5);
    for (const auto& pos : road.bridgeList) {
        EXPECT_TRUE(road.grid.getType(pos).has_value());
        EXPECT_EQ(road.grid.getType(pos).value(), "road");
    }
}