#pragma once

#include "Entity.hpp"
#include "Texture.hpp"
#include "Seeder.hpp"

namespace Factory
{

	Entity &createGlobal();
	//Entity &createCow();
	Entity& createPoacher();
	Entity& createCarcass(Vec3 spawnPos, std::string killerLabel, Texture texture);

	Entity& createHunter(Vec3 spawnPos, int hutId, int localId, std::string label);
	Entity& createHunterHut(Vec3 spawnPos);
	
	Entity& createDroneStation(Vec3 spawnPos);
	Entity& createDrone(Vec3 spawnPos, Entity* parent);

	Entity& createAirship(Vec3 spawnPos);
	Entity& createSecurityCamera(Vec3 spawnPos);

	//Entity &createMovableCow();

	Entity &createZebraHerd();
	Entity &createZebra(int herdId, int localId, const std::string& label);

	Entity& createWildebeestHerd();
	Entity& createWildebeest(int herdId, int localId, const std::string& label);

	Entity& createGiraffeHerd();
	Entity& createGiraffe(int herdId, int localId, const std::string& label);

	Entity& createLionPride();
	Entity& createLion(int herdId, int localId, const std::string& label);

	Entity& createTestHerbivoreHerd();
	Entity& createTestHerbivore(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel);


	Entity& createTree1();

	Entity& createTreeTest(std::string path);

	Entity& createRandomSpookyCrap();

	Entity& createJeep(Vec3 spawnPos);

	Entity& createEcosystem();

	Entity& createTree2();

	Entity& createCactus1();

	Entity& createCactus2();

	Entity& createCactus3();

	Entity& createSiteMarker(Vec3 position);

}