#include "Seeder.hpp"
#include "Core.hpp"

void Seeder::seedMenu()
{
	//Labels::getUnique("global").addComponent<Time>();

	//Factory::createAirship(Vec3(10.0f, 8.0f, 10.0f));

	//Entity& ecosystem = Factory::createEcosystem();
	
	Entity& ecosystem = Factory::createEcosystem();
	HerdManager& herdManager = ecosystem.getComponent<HerdManager>();
	herdManager.createZebraHerd(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 5);
	herdManager.createZebraHerd(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 3);
	herdManager.createWildebeestHerd(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 8);

	for (int i = 0; i < 26; ++i) {

		Factory::createTree1();

		//Factory::createTreeTest("tree8.png");
		//Factory::createTreeTest("tempTree2.png");
		//Factory::createTreeTest("tempTree3.png");
		//Factory::createTreeTest("tempTree4.png");
		//Factory::createTreeTest("tempTree5.png");

	}

	for (int i = 0; i < 50; ++i)
	{
		//Labels::getUnique("eco").getComponent<FloraManager>().generateInitialPlants(1.0f);
	}



	//Labels::getUnique("global").getComponent<GameManager>().startGame();
}

void Seeder::seedMain()
{
	Entity& ecosystem = Factory::createEcosystem();
	HerdManager& herdManager = ecosystem.getComponent<HerdManager>();
	herdManager.createZebraHerd(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 5);
	herdManager.createZebraHerd(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 3);
	herdManager.createWildebeestHerd(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 8);
	herdManager.createWildebeestHerd(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 12);
	herdManager.createLionPride(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 4);
	herdManager.createHyenaPack(Vec3(randf(-15.0f, 15.0f), 0.0f, randf(-15.0f, 15.0f)), 6);
	FloraManager& floraManager = ecosystem.getComponent<FloraManager>();
	floraManager.setInitialPlantCount(EdiblePlantTypes::NormalGrass, 4);
	floraManager.setInitialPlantCount(EdiblePlantTypes::ThickGrass, 10);
	floraManager.setInitialPlantCount(EdiblePlantTypes::RedGrass, 1);
	floraManager.setInitialPlantCount(EdiblePlantTypes::ExoticGrass, 0);
	floraManager.setInitialPlantCount(EdiblePlantTypes::EdibleTree, 0);





	for (int i = 0; i < 50; ++i)
	{
		ecosystem.getComponent<FloraManager>().generateInitialPlants(1.0f);
	}











	//Labels::getUnique("global").addComponent<Time>();
    
	//Factory::createTestCollisionSpace();

	/*Entity& zH = Factory::createZebraHerd();
	for (int i = 0; i < 10; ++i) {

		auto [herdId, localId, label] = zH.getComponent<ZebraHerd>().addAnimal();
		Factory::createZebra(herdId, localId, label);

	}

	/*Entity& zH2 = Factory::createZebraHerd();
	for (int i = 0; i < 5; ++i) {

		auto [herdId, localId, label] = zH2.getComponent<ZebraHerd>().addAnimal();
		Factory::createZebra(herdId, localId, label);

	}

	Entity& wH = Factory::createWildebeestHerd();
	for (int i = 0; i < 16; ++i) {

		auto [herdId, localId, label] = wH.getComponent<WildebeestHerd>().addAnimal();
		Factory::createWildebeest(herdId, localId, label);

	}

	Entity& gH = Factory::createGiraffeHerd();
	for (int i = 0; i < 6; ++i) {

		auto [herdId, localId, label] = gH.getComponent<GiraffeHerd>().addAnimal();
		Factory::createGiraffe(herdId, localId, label);

	}


	Entity& lP = Factory::createLionPride();
	for (int i = 0; i < 4; ++i) {

		auto [herdId, localId, label] = lP.getComponent<LionPride>().addAnimal();
		Factory::createLion(herdId, localId, label);

	}


	Entity& cP = Factory::createTestHerbivoreHerd();
	for (int i = 0; i < 30; ++i) {

		auto [herdId, localId, herdLabel, animalLabel] = cP.getComponent<HerbivoreHerdAI>().addAnimal();
		Factory::createTestHerbivore(herdId, localId, herdLabel, animalLabel);

	}
	*/


	for (int i = 0; i < 18; ++i) {

		Factory::createTree1();

	}

	for (int i = 0; i < 7; ++i) {

		Factory::createTree2();

	}

	for (int i = 0; i < 9; ++i) {

		Factory::createCactus1();

	}

	for (int i = 0; i < 5; ++i) {

		Factory::createCactus2();

	}

	for (int i = 0; i < 2; ++i) {

		Factory::createCactus3();

	}

	//for (int i = 0; i < 20; i++)
	//{
	//	Factory::createPoacher();
	//}


	//Factory::createRandomSpookyCrap();

	
	Labels::getUnique("global").getComponent<GameManager>().setGameStarted(true);
}

void Seeder::resetWorld()
{
	Core::destroyAllEntities();
}