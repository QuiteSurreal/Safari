#include "Road.hpp"
#include "Behaviour.hpp"
#include "Input.hpp"
#include "Transform.hpp"
#include "Labels.hpp"
#include "GridSelector.hpp"
#include "Grid.hpp"
#include "Texture.hpp"
#include "UIBehaviour.hpp"
#include <random>
#include <queue>
#include <algorithm>
#include <glm/glm.hpp>
#include <functional>

#include <unordered_map>
#include <unordered_set>
#include "Factory.hpp"

void Road::Reset() {

	bridgeList.clear();

	for (int i = 0; i < grid.getSize().x; ++i) {
		for (int j = 0; j < grid.getSize().y; ++j) {
			grid.setTexture({i, j}, RandomTerrainTexture());
			grid.setType({i, j}, {});
		}
	}

	//Generate a random entry point
	int randomNumber = std::rand() % 50; 

	grid.setTexture(Ivec2(randomNumber,0), Texture::fromImage("RoadEntry.png"));

	entryPoint = Ivec2(randomNumber, 0);

	//Generate a random exit point
	randomNumber = std::rand() % 50;

	grid.setTexture(Ivec2(randomNumber, 49), Texture::fromImage("RoadExit.png"));

	exitPoint = Ivec2(randomNumber, 49);

	isConnected = false;

	GenerateRiver();

}

void Road::start(Entity&) {

	Reset();

}


void Road::update(Entity& self) {

	if (Input::isKeyPressed(Input::Key::K)) {
			//Start a jeep here
			Factory::createJeep(Vec3(entryPoint.x-25+0.5, 0, -25));

			if (GetRoadList().size()>=1) {
				std::string s = "Roads:\n";
		
				for (Ivec2 tile : GetRoadList()) {
					s.append(std::to_string(tile.x));
					s.append(" ");
					s.append(std::to_string(tile.y));
					s.append("\n");
				}
		
				logDebug(s.c_str());
			}

	}

		
	if (uib.mode) {

		if (*uib.mode == "structure1") {
			//Placing Roads
			for (Ivec2 tile : gs.getClicks(Input::MouseButton::Left)) {
				PlaceRoad(tile.x, tile.y);
				//logDebug(PrintRoadList());
			}

			//Check for delete
			for (Ivec2 tile : gs.getClicks(Input::MouseButton::Right)) {
				if (grid.getType(tile) == "road") {

					DeleteRoad(tile.x, tile.y);

				}
					
			}
		}
		else if (*uib.mode == "structure6") {
			//Placing Water
			for (Ivec2 tile : gs.getClicks(Input::MouseButton::Left)) {
				PlaceWater(tile.x, tile.y);
			}

			//Check for delete
			for (Ivec2 tile : gs.getClicks(Input::MouseButton::Right)) {
				if (grid.getType(tile) == "water") {

					DeleteWater(tile.x, tile.y);
					
				}
					
			}
		}


	}
}

void Road::DeleteRoad(int X, int Y)
{
	if(grid.getType(Ivec2(X, Y)) != "road"){
		return;
	}

	if(std::find(bridgeList.begin(), bridgeList.end(), Ivec2(X,Y)) != bridgeList.end()){
		return;
	}

	grid.setTexture(Ivec2(X, Y), RandomTerrainTexture());
	grid.setType(Ivec2(X, Y), {});

	UpdateRoadNeighbours(X, Y);
}


void Road::DeleteWater(int X, int Y)
{

	if(grid.getType(Ivec2(X, Y)) != "water"){
		return;
	}

	grid.setTexture(Ivec2(X, Y), RandomTerrainTexture());
	grid.setType(Ivec2(X, Y), {});

	UpdateWaterNeighbours(X, Y);
}

void Road::GenerateRiver()
{
	int bridgeCount = 5;

    std::vector<Ivec2> riverTiles;  // Track river path for possible bridge positions
    bridgeList.clear();             // Clear any existing bridges

    // Generate Y positions (heights) for each horizontal segment
    int y1 = std::rand() % 10 + 2;
    int y2 = std::rand() % 10 + 14;
    int y3 = std::rand() % 10 + 26;
    int y4 = std::rand() % 10 + 38;

    // Generate X positions where the river turns
    int x1 = std::rand() % 10 + 10;
    int x2 = std::rand() % 10 + 20;
    int x3 = std::rand() % 10 + 35;

    auto setWater = [&](Ivec2 pos, const std::string& tex) {
        grid.setTexture(pos, Texture::fromImage(tex));
        grid.setType(pos, "water");
        riverTiles.push_back(pos); // Save tile for bridge possibility
    };

    // 1. First horizontal segment
    for (int x = 0; x < x1; ++x)
        setWater(Ivec2(x, y1), "WaterHorizontal.png");

    // 2. First corner
    setWater(Ivec2(x1, y1), (y2 > y1) ? "WaterBottomLeft.png" : "WaterTopLeft.png");

    // 3. First vertical segment
    for (int y = std::min(y1, y2) + 1; y < std::max(y1, y2); ++y)
        setWater(Ivec2(x1, y), "WaterVertical.png");

    // 4. Second corner
    setWater(Ivec2(x1, y2), (y2 > y1) ? "WaterTopRight.png" : "WaterBottomRight.png");

    // 5. Second horizontal segment
    for (int x = x1 + 1; x < x2; ++x)
        setWater(Ivec2(x, y2), "WaterHorizontal.png");

    // 6. Third corner
    setWater(Ivec2(x2, y2), (y3 > y2) ? "WaterBottomLeft.png" : "WaterTopLeft.png");

    // 7. Second vertical segment
    for (int y = std::min(y2, y3) + 1; y < std::max(y2, y3); ++y)
        setWater(Ivec2(x2, y), "WaterVertical.png");

    // 8. Fourth corner
    setWater(Ivec2(x2, y3), (y3 > y2) ? "WaterTopRight.png" : "WaterBottomRight.png");

    // 9. Third horizontal segment
    for (int x = x2 + 1; x < x3; ++x)
        setWater(Ivec2(x, y3), "WaterHorizontal.png");

    // 10. Fifth corner
    setWater(Ivec2(x3, y3), (y4 > y3) ? "WaterBottomLeft.png" : "WaterTopLeft.png");

    // 11. Third vertical segment
    for (int y = std::min(y3, y4) + 1; y < std::max(y3, y4); ++y)
        setWater(Ivec2(x3, y), "WaterVertical.png");

    // 12. Sixth corner
    setWater(Ivec2(x3, y4), (y4 > y3) ? "WaterTopRight.png" : "WaterBottomRight.png");

    // 13. Final horizontal segment
    for (int x = x3 + 1; x <= 49; ++x)
        setWater(Ivec2(x, y4), "WaterHorizontal.png");

    // --- Add Random Bridges ---
    std::shuffle(riverTiles.begin(), riverTiles.end(), std::mt19937{ std::random_device{}() });

    int bridgesPlaced = 0;
    for (const Ivec2& pos : riverTiles) {
        std::string currentType = grid.getType(pos).value();  // Must be "water"
        if (currentType != "water")
            continue;

        Texture currentTex = grid.getTexture(pos).value();
        std::string bridgeTex;

        // Infer orientation from existing water tile
        if (currentTex ==  Texture::fromImage("WaterHorizontal.png"))
            bridgeTex = "RoadVertical.png"; // bridge is vertical (across)
        else if (currentTex == Texture::fromImage("WaterVertical.png"))
            bridgeTex = "RoadHorizontal.png"; // bridge is horizontal (across)
        else
            continue; // Skip corners

        // Set bridge
        grid.setTexture(pos, Texture::fromImage(bridgeTex));
		grid.setType(pos, "road");
		UpdateRoad(pos.x, pos.y);
        bridgeList.push_back(pos);

        if (++bridgesPlaced >= bridgeCount)
            break;

		UpdateRoad(pos.x, pos.y);
    }
}



bool Road::PlaceRoad(int X, int Y) {

	if(X == entryPoint.x && Y == entryPoint.y){
		return false;
	}

	if(X == exitPoint.x && Y == exitPoint.y){
		return false;
	}

	if(grid.getType(Ivec2(X, Y)) != std::nullopt){
		return false;
	}

	Texture t = Texture::fromImage("RoadHorizontal.png");
	grid.setTexture(Ivec2(X, Y), t);
	grid.setType(Ivec2(X,Y), "road");

	UpdateRoad(X,Y);
	UpdateRoadNeighbours(X, Y);

	return true;
}

bool Road::PlaceWater(int X, int Y) {

	if(X == entryPoint.x && Y == entryPoint.y){
		return false;
	}

	if(X == exitPoint.x && Y == exitPoint.y){
		return false;
	}

	if(grid.getType(Ivec2(X, Y)) != std::nullopt){
		return false;
	}

	Texture t = Texture::fromImage("WaterHorizontal.png");
	grid.setTexture(Ivec2(X, Y), t);
	grid.setType(Ivec2(X,Y), "water");

	UpdateWater(X,Y);
	UpdateWaterNeighbours(X, Y);

	return true;
}

void Road::UpdateRoadNeighbours(int X, int Y){
	UpdateRoad(X + 1,Y);
	UpdateRoad(X - 1,Y);
	UpdateRoad(X,Y + 1);
	UpdateRoad(X,Y - 1);
}

void Road::UpdateRoad(int X, int Y) {

	if(!IsInBound(X, Y)){
		return;
	}

	if(grid.getType(Ivec2(X,Y)) != "road"){
		return;
	}

	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;

	int neighbourCount = 0;

	//left
	if(IsInBound(X - 1, Y)){
		if(grid.getType(Ivec2(X-1, Y)) == "road"){
			left = true;
			neighbourCount++;
		}
	}

	//right
	if(IsInBound(X + 1, Y)){
		if(grid.getType(Ivec2(X + 1, Y)) == "road"){
			right = true;
			neighbourCount++;
		}
	}

	//up
	if(IsInBound(X, Y - 1)){
		if(grid.getType(Ivec2(X, Y - 1)) == "road"){
			up = true;
			neighbourCount++;
		}
	}

	//down
	if(IsInBound(X, Y + 1)){
		if(grid.getType(Ivec2(X, Y + 1)) == "road"){
			down = true;
			neighbourCount++;
		}
	}

	std::string textureName = "RoadVertical.png";
	if(IsInBound(X, Y-1) && IsInBound(X, Y+1)){
		if(grid.getType(Ivec2(X,Y-1)) == "water" && grid.getType(Ivec2(X,Y+1)) == "water"){
			textureName = "RoadHorizontal.png";
		}
	}

	if(neighbourCount == 4){
		textureName = "RoadFull.png";
	}
	else if(neighbourCount == 3){
		if (!up) textureName = "RoadNoTop.png";
        else if (!down) textureName = "RoadNoBottom.png";
        else if (!left) textureName = "RoadNoLeft.png";
        else if (!right) textureName = "RoadNoRight.png";
	}
	else if(neighbourCount == 2){
		if(left && right){
			textureName = "RoadHorizontal.png";
		}
		else if(up && down){
			textureName = "RoadVertical.png";
		}
		else if(up && left){
			textureName = "RoadTopLeft.png";
		}
		else if(up && right){
			textureName = "RoadTopRight.png";
		}
		else if(down && left){
			textureName = "RoadBottomLeft.png";
		}
		else if(down && right){
			textureName = "RoadBottomRight.png";
		}
	}
	else if(neighbourCount == 1){
		if(up){
			textureName = "RoadVertical.png";
		}
		else if(down){
			textureName = "RoadVertical.png";
		}
		else if(left){
			textureName = "RoadHorizontal.png";
		}
		else if(right){
			textureName = "RoadHorizontal.png";
		}
	}

	grid.setTexture(Ivec2(X, Y), Texture::fromImage(textureName));
}

void Road::UpdateWaterNeighbours(int X, int Y){
	UpdateWater(X + 1,Y);
	UpdateWater(X - 1,Y);
	UpdateWater(X,Y + 1);
	UpdateWater(X,Y - 1);
}

void Road::UpdateWater(int X, int Y) {

    if (!IsInBound(X, Y)) {
        return;
    }

    if (grid.getType(Ivec2(X, Y)) != "water") {
        return;
    }

    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;

    int neighbourCount = 0;

    // Check left
    if (IsInBound(X - 1, Y) && grid.getType(Ivec2(X - 1, Y)) == "water") {
        left = true;
        neighbourCount++;
    }

    // Check right
    if (IsInBound(X + 1, Y) && grid.getType(Ivec2(X + 1, Y)) == "water") {
        right = true;
        neighbourCount++;
    }

    // Check up
    if (IsInBound(X, Y - 1) && grid.getType(Ivec2(X, Y - 1)) == "water") {
        up = true;
        neighbourCount++;
    }

    // Check down
    if (IsInBound(X, Y + 1) && grid.getType(Ivec2(X, Y + 1)) == "water") {
        down = true;
        neighbourCount++;
    }

    std::string textureName = "WaterFull.png";

    if (neighbourCount == 4) {
        textureName = "WaterFull.png";
    } else if (neighbourCount == 3) {
        if (!up) textureName = "WaterNoTop.png";
        else if (!down) textureName = "WaterNoBottom.png";
        else if (!left) textureName = "WaterNoLeft.png";
        else if (!right) textureName = "WaterNoRight.png";
    } else if (neighbourCount == 2) {
        if (left && right) {
            textureName = "WaterHorizontal.png";
        } else if (up && down) {
            textureName = "WaterVertical.png";
        } else if (up && left) {
            textureName = "WaterTopLeft.png";
        } else if (up && right) {
            textureName = "WaterTopRight.png";
        } else if (down && left) {
            textureName = "WaterBottomLeft.png";
        } else if (down && right) {
            textureName = "WaterBottomRight.png";
        }
    } else if (neighbourCount == 1) {
        if (up || down) {
            textureName = "WaterVertical.png";
        } else if (left || right) {
            textureName = "WaterHorizontal.png";
        }
    } else {
        textureName = "WaterHorizontal.png"; // Optional: texture for isolated tile
    }

    grid.setTexture(Ivec2(X, Y), Texture::fromImage(textureName));
}


bool Road::IsInBound(int X, int Y)
{
	if(X < 0 || X > 49){
		return false;
	}

	if(Y < 0 || Y > 49){
		return false;
	}

	return true;
}


std::vector<Ivec2> Road::GetRoadList()
{
    std::vector<Ivec2> fullRoadList = grid.getAllOfType("road");
    fullRoadList.push_back(entryPoint);
    fullRoadList.push_back(exitPoint);

    std::unordered_set<glm::ivec2> roadSet(fullRoadList.begin(), fullRoadList.end());
    std::unordered_set<glm::ivec2> visited;
    std::unordered_map<glm::ivec2, glm::ivec2> cameFrom;

    const std::vector<Ivec2> directions = {
        Ivec2(0, 1), Ivec2(1, 0), Ivec2(0, -1), Ivec2(-1, 0)
    };

    std::function<bool(Ivec2)> dfs = [&](Ivec2 current) -> bool {
        visited.insert(current);

        if (current == exitPoint)
            return true;

        std::vector<Ivec2> shuffledDirs = directions;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(shuffledDirs.begin(), shuffledDirs.end(), g);

        for (const Ivec2& dir : shuffledDirs) {
            Ivec2 neighbor = current + dir;
            if (roadSet.count(neighbor) && visited.count(neighbor) == 0) {
                cameFrom[neighbor] = current;
                if (dfs(neighbor))
                    return true;
            }
        }

        return false;
    };

    if (!dfs(entryPoint)) {
        return {}; // No path found
    }

    std::vector<Ivec2> roadList;
    Ivec2 current = exitPoint;
    while (current != entryPoint) {
        roadList.push_back(current);
        current = cameFrom[current];
    }
    roadList.push_back(entryPoint);
    std::reverse(roadList.begin(), roadList.end());
    return roadList;
}

Texture Road::RandomTerrainTexture()
{
	static const std::vector<std::string> textures = {
		"gr1.png",
		"gr2.png",
		"gr3.png",
		"gr4.png",
	};
	const std::string &texture = textures[rand() % textures.size()];
	return Texture::fromImage(texture);
}
