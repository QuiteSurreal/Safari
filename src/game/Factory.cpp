#include "Factory.hpp"
#include "Core.hpp"
#include "Background.hpp"
#include "Time.hpp"
#include "UI.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Labels.hpp"
#include "UIBehaviour.hpp"
#include "Cow.hpp"
#include "Grid.hpp"
#include "Terrain.hpp"
#include "Sprite.hpp"
#include "InteractableObjects/Poacher.hpp"
#include "DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "./Animals/Zebra.hpp"
#include "./Animals/ZebraHerd.hpp"
#include "./TestBehaviours/AoECow.hpp"
#include "./Animals/RandomMigration.hpp"
#include "./Animals/Wildebeest.hpp"
#include "./Animals/WildebeestHerd.hpp"
#include "./Animals/Giraffe.hpp"
#include "./Animals/GiraffeHerd.hpp"
#include "./Animals/Lion.hpp"
#include "./Animals/LionPride.hpp"

#include "HerdManager.hpp"
#include "AnimalManager.hpp"
#include "FloraManager.hpp"

#include "./AI/HerbivoreAI.hpp"
#include "./AI/HerbivoreHerdAI.hpp"

#include "GridSelector.hpp"
#include "InteractableObjects/EntitySelector.hpp"
#include "Road.hpp"
#include "Texture.hpp"
#include "Health.hpp"
#include "InteractableObjects/DroneStation.hpp"
#include "InteractableObjects/Drone.hpp"
#include "InteractableObjects/Airship.hpp"
#include "InteractableObjects/SecurityCamera.hpp"
#include "InteractableObjects/HunterHut.hpp"
#include "InteractableObjects/Hunter.hpp"
#include "Selector.hpp"
#include "Jeep.hpp"
#include "Targetable.hpp"
#include "GameManager.hpp"


namespace Factory
{

	static int globalZId = 1;
	static int globalPoacherId = 1;
	static int globalDroneId = 1;
	static int globalHutId = 1;

	static int globalJeepId = 1;
	
	Entity &createGlobal()
	{
		Entity &entity = Core::createEntity(true);
		entity.addComponent<Time>();
		entity.addComponent<UI>();
		entity.addComponent<UIBehaviour>();
		entity.addComponent<Camera>();
		entity.addComponent<Light::CameraLight>();
		entity.addComponent(Background {Vec3 {0.0f, 1.0f, 1.0f}});
		entity.addComponent<Labels>().addLabel("global");
		const auto &grid = entity.addComponent(Grid {Ivec2 {50}, 1.0f});
		entity.addComponent(Terrain {grid, Texture::fromImage("gr1.png")});
		entity.addComponent<GridSelector>().enable();
		entity.addComponent<EntitySelector>().enable();
		entity.addComponent<Road>();
		entity.addComponent<GameManager>();
		return entity;
	}
	
	/*Entity& createCow()
	{
		const Texture texture = Texture::fromImage("cow.png");
		auto &entity = Core::createEntity();
		auto &transform = entity.addComponent<Transform>();
		entity.addComponent(Sprite {transform, texture});
		entity.addComponent(Light::TransformedLight {transform, 2.5f});
		entity.addComponent<Cow>();
		entity.addComponent<Labels>().addLabel("cow");
		return entity;
	}*/

	Entity& createPoacher()
	{
		const Texture texture = Texture::fromImage("Poacher.png"); // TODO
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		entity.addComponent(Sprite{ transform, texture });
		entity.getComponent<Sprite>().setOpacity(0.0f);

		std::vector<float> edge = { -25.0f, 25.0f };
		int randEdge = rand() % 2;
		int randPoz = randf(-25.0f, +25.0f);

		Transform& trans = entity.getComponent<Transform>();

		int randChoice = rand() % 2;
		if (randChoice == 0)
		{
			trans.relocate({ edge[randEdge], 0.0f, randPoz });
		}
		else
		{
			trans.relocate({ randPoz, 0.0f, edge[randEdge] });
		}

		entity.addComponent<Poacher>(entity.getComponent<Transform>().getPosition());

		entity.addComponent<Labels>();
		entity.getComponent<Poacher>().setUniqueId(globalPoacherId);
		globalPoacherId++;
		entity.getComponent<Labels>().addLabel("poacher");
		entity.getComponent<Labels>().addLabel("Spooky");

		//entity.addComponent<Cow>();

		entity.addComponent<Health>();
		entity.getComponent<Health>().setHealth(3);

		entity.addComponent<Targetable>();

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.08f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(0.0f, 0.0f, 0.0f));

		

		return entity;
	}

	Entity& createDroneStation(Vec3 spawnPos)
	{
		const Texture texture = Texture::fromImage("droneStation.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		entity.addComponent(Sprite{ transform, texture });
		entity.getComponent<Transform>().relocate(spawnPos);

		entity.addComponent<Labels>();

		entity.addComponent<DroneStation>(spawnPos);
		entity.getComponent<DroneStation>().setUniqueId(globalDroneId);
		globalDroneId++;

		

		return entity;
	}

	Entity& createDrone(Vec3 spawnPos, Entity* parent)
	{
		const Texture texture = Texture::fromImage("drone.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		entity.addComponent(Sprite{ transform, texture });
		entity.getComponent<Transform>().relocate(spawnPos);

		entity.addComponent<Labels>();

		entity.addComponent<Drone>(spawnPos);
		entity.getComponent<Drone>().setParent(parent);

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.04f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(0.0f, 0.0f, 0.0f));

		

		return entity;
	}

	Entity& createAirship(Vec3 spawnPos)
	{
		const Texture texture = Texture::fromImage("airship.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();


		entity.addComponent(Sprite{ transform, texture });
		entity.getComponent<Transform>().relocate(Vec3(spawnPos.x, 6.5f, spawnPos.z));

		entity.addComponent<Labels>();

		entity.addComponent<Airship>();

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.04f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(0.0f, 0.0f, 0.0f));

		

		return entity;
	}

	Entity& createSecurityCamera(Vec3 spawnPos)
	{
		const Texture texture = Texture::fromImage("camera.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();


		entity.addComponent(Sprite{ transform, texture });
		entity.getComponent<Transform>().relocate(spawnPos);

		entity.addComponent<Labels>();

		entity.addComponent<SecurityCamera>();

		

		return entity;
	}

	Entity& createHunter(Vec3 spawnPos, int hutId, int localId, std::string label)
	{
		const Texture texture = Texture::fromImage("hunter.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();


		entity.addComponent(Sprite{ transform, texture });
		entity.getComponent<Transform>().relocate(spawnPos);

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(label);
		entity.getComponent<Labels>().addLabel("hunter");

		entity.addComponent<Hunter>(spawnPos);
		//entity.getComponent<Hunter>().setStartingPos(spawnPos);
		entity.getComponent<Hunter>().setHutId(hutId);
		entity.getComponent<Hunter>().setLocalId(localId);
		entity.getComponent<Hunter>().setHunterLabel(label);

		entity.addComponent<Selector>(1.0f);

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.04f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(0.0f, 0.0f, 0.0f));

		

		return entity;
	}

	Entity& createHunterHut(Vec3 spawnPos)
	{
		const Texture texture = Texture::fromImage("hunterHut.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();


		entity.addComponent(Sprite{ transform, texture });
		entity.getComponent<Transform>().relocate(spawnPos);

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel("hunterHut");

		entity.addComponent<HunterHut>(spawnPos);
		entity.getComponent<HunterHut>().setUniqueId(globalHutId);
		globalHutId++;

		

		return entity;
	}


	Entity& createCarcass(Vec3 spawnPos, std::string killerLabel, Texture texture)
	{
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		entity.addComponent(Sprite{ transform, texture });
		entity.getComponent<Transform>().relocate(spawnPos);

		entity.addComponent<Labels>();
		if (killerLabel != "")
		{
			entity.getComponent<Labels>().addLabel("carcass:" + killerLabel);
		}
		else
		{
			entity.getComponent<Labels>().addLabel("carcass");
		}
		entity.getComponent<Labels>().addLabel("Spooky");

		entity.addComponent<Carcass>();

		entity.addComponent<Targetable>();

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.02f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(0.0f, 0.0f, 0.0f));

		

		return entity;
	}

	/*Entity& createMovableCow()
	{
		const Texture texture = Texture::fromImage("cow.png"); // DO
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();
		entity.addComponent(Sprite{ transform, texture });
		entity.addComponent(Light::TransformedLight{ transform, 2.5f });
		entity.addComponent<Cow>();
		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.02f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(1.0f, 1.0f, 1.0f));
		entity.getComponent<DestinationBasedMovement>().updatePath();
		entity.addComponent<AoECow>();
		return entity;

	}*/

	Entity& createZebraHerd() {

		auto& entity = Core::createEntity();

		entity.addComponent<ZebraHerd>();
		entity.getComponent<ZebraHerd>().assignUniqueHerdId(globalZId);
		globalZId++;
		entity.getComponent<ZebraHerd>().setFOVRadius(3);
		entity.getComponent<ZebraHerd>().setRoamRadius(6);
		entity.getComponent<ZebraHerd>().setMigrateRadius(3);

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(entity.getComponent<ZebraHerd>().getHerdLabel());
		entity.getComponent<Labels>().addLabel("zebraHerd");

		//entity.addComponent<Cow>(); // test

		entity.addComponent<Transform>();
		entity.getComponent<Transform>().relocate(Vec3(randf(-20.0f,20.0f), 0.0f, randf(-20.0f, 20.0f)));

		entity.addComponent<Targetable>();

		//entity.addComponent(Light::TransformedLight{ entity.getComponent<Transform>(), 2.5f}); // test

		//const Texture texture = textureFromImage("cow.png"); // test
		//entity.addComponent(Sprite{ entity.addComponent<Transform>(), texture }); // test

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.015f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

		entity.addComponent<RandomMigration>();

		

		return entity;

	}

	Entity& createZebra(int herdId, int localId, const std::string& label) {

		const Texture texture = Texture::fromImage("zebra.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		entity.addComponent(Sprite{ transform, texture });
		entity.addComponent(Light::TransformedLight{ transform, 0.0f });

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(label);
		entity.getComponent<Labels>().addLabel("animal");
		entity.getComponent<Labels>().addLabel("zebra"); //TODO

		entity.addComponent<Cow>();

		entity.addComponent<Zebra>();
		entity.getComponent<Zebra>().setHerdId(herdId);
		entity.getComponent<Zebra>().setLocalId(localId);

		entity.addComponent<Health>();
		entity.getComponent<Health>().setHealth(1);

		entity.addComponent<Selector>(1.0f);

		entity.addComponent<Targetable>();

		// TEMP
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<int> dist(0, 20000);
		// TEMP


		entity.getComponent<Zebra>().setIdleMax(20480 + dist(gen));
		entity.getComponent<Zebra>().setFieldOfView(2.6f);

		// TEMP NO ONE SAW THAT
		static std::random_device rd2;
		static std::mt19937 gen2(rd2());
		static std::uniform_int_distribution<int> dist2(0, 120);
		// TEMP

		entity.getComponent<Zebra>().setSpookMax(50 + dist2(gen2));



		Labels::getUnique("global").getComponent<GameManager>().addAnimal();


		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.02f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(0.0f, 0.0f, 0.0f));

		

		return entity;

	}



	Entity& createWildebeestHerd() {

		auto& entity = Core::createEntity();

		entity.addComponent<WildebeestHerd>();
		entity.getComponent<WildebeestHerd>().assignUniqueHerdId(1);
		entity.getComponent<WildebeestHerd>().setFOVRadius(3);
		entity.getComponent<WildebeestHerd>().setRoamRadius(6);
		entity.getComponent<WildebeestHerd>().setMigrateRadius(3);

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(entity.getComponent<WildebeestHerd>().getHerdLabel());
		entity.getComponent<Labels>().addLabel("wildebeestHerd");

		//entity.addComponent<Cow>(); // test

		entity.addComponent<Transform>();
		entity.getComponent<Transform>().relocate(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

		//entity.addComponent(Light::TransformedLight{ entity.getComponent<Transform>(), 2.5f }); // test

		//const Texture texture = textureFromImage("cow.png"); // test
		//entity.addComponent(Sprite{ entity.addComponent<Transform>(), texture }); // test

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.015f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

		entity.addComponent<RandomMigration>();

		

		return entity;

	}

	Entity& createWildebeest(int herdId, int localId, const std::string& label) {

		const Texture texture = Texture::fromImage("wildebeest.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		entity.addComponent(Sprite{ transform, texture });
		//entity.addComponent(Light::TransformedLight{ transform, 2.5f });

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(label);
		entity.getComponent<Labels>().addLabel("animal");
		entity.getComponent<Labels>().addLabel("wildebeest");

		entity.addComponent<Cow>();

		entity.addComponent<Wildebeest>();
		entity.getComponent<Wildebeest>().setHerdId(herdId);
		entity.getComponent<Wildebeest>().setLocalId(localId);



		// TEMP
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<int> dist(0, 20000);
		// TEMP


		entity.getComponent<Wildebeest>().setIdleMax(20480 + dist(gen));
		entity.getComponent<Wildebeest>().setFieldOfView(2.6f);

		// TEMP NO ONE SAW THAT
		static std::random_device rd2;
		static std::mt19937 gen2(rd2());
		static std::uniform_int_distribution<int> dist2(0, 120);
		// TEMP

		entity.getComponent<Wildebeest>().setSpookMax(50 + dist2(gen2));



		Labels::getUnique("global").getComponent<GameManager>().addAnimal();


		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.02f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(0.0f, 0.0f, 0.0f));

		

		return entity;

	}




	Entity& createGiraffeHerd() {

		auto& entity = Core::createEntity();

		entity.addComponent<GiraffeHerd>();
		entity.getComponent<GiraffeHerd>().assignUniqueHerdId(1);
		entity.getComponent<GiraffeHerd>().setFOVRadius(3);
		entity.getComponent<GiraffeHerd>().setRoamRadius(6);
		entity.getComponent<GiraffeHerd>().setMigrateRadius(3);

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(entity.getComponent<GiraffeHerd>().getHerdLabel());
		entity.getComponent<Labels>().addLabel("giraffeHerd");

		//entity.addComponent<Cow>(); // test

		entity.addComponent<Transform>();
		entity.getComponent<Transform>().relocate(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

		//entity.addComponent(Light::TransformedLight{ entity.getComponent<Transform>(), 2.5f }); // test

		//const Texture texture = textureFromImage("cow.png"); // test
		//entity.addComponent(Sprite{ entity.addComponent<Transform>(), texture }); // test

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.01f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

		entity.addComponent<RandomMigration>();

		

		return entity;

	}

	Entity& createGiraffe(int herdId, int localId, const std::string& label) {

		const Texture texture = Texture::fromImage("giraffe.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		entity.addComponent(Sprite{ transform, texture });
		//entity.addComponent(Light::TransformedLight{ transform, 2.5f });

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(label);
		entity.getComponent<Labels>().addLabel("animal");
		entity.getComponent<Labels>().addLabel("giraffe");

		entity.addComponent<Cow>();

		entity.addComponent<Giraffe>();
		entity.getComponent<Giraffe>().setHerdId(herdId);
		entity.getComponent<Giraffe>().setLocalId(localId);
		



		// TEMP
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<int> dist(0, 20000);
		// TEMP


		entity.getComponent<Giraffe>().setIdleMax(20480 + dist(gen));
		entity.getComponent<Giraffe>().setFieldOfView(2.6f);

		// TEMP NO ONE SAW THAT
		static std::random_device rd2;
		static std::mt19937 gen2(rd2());
		static std::uniform_int_distribution<int> dist2(0, 120);
		// TEMP

		Labels::getUnique("global").getComponent<GameManager>().addAnimal();

		entity.getComponent<Giraffe>().setSpookMax(50 + dist2(gen2));


		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.02f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(0.0f, 0.0f, 0.0f));

		
		
		return entity;

	}


	Entity& createLionPride() {

		auto& entity = Core::createEntity();

		entity.addComponent<LionPride>();
		entity.getComponent<LionPride>().assignUniqueHerdId(1);
		entity.getComponent<LionPride>().setFOVRadius(3);
		entity.getComponent<LionPride>().setRoamRadius(6);
		entity.getComponent<LionPride>().setMigrateRadius(3);

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(entity.getComponent<LionPride>().getHerdLabel());
		entity.getComponent<Labels>().addLabel("lionPride");

		//entity.addComponent<Cow>(); // test

		entity.addComponent<Transform>();
		entity.getComponent<Transform>().relocate(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

		//entity.addComponent(Light::TransformedLight{ entity.getComponent<Transform>(), 2.5f }); // test

		//const Texture texture = textureFromImage("cow.png"); // test
		//entity.addComponent(Sprite{ entity.addComponent<Transform>(), texture }); // test

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.01f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

		entity.addComponent<RandomMigration>();

		

		return entity;

	}

	Entity& createLion(int herdId, int localId, const std::string& label) {

		const Texture texture = Texture::fromImage("lion.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		entity.addComponent(Sprite{ transform, texture });
		//entity.addComponent(Light::TransformedLight{ transform, 2.5f });

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(label);
		entity.getComponent<Labels>().addLabel("Spooky");
		entity.getComponent<Labels>().addLabel("animal");
		entity.getComponent<Labels>().addLabel("lion");

		entity.addComponent<Cow>();

		entity.addComponent<Lion>();
		entity.getComponent<Lion>().setHerdId(herdId);
		entity.getComponent<Lion>().setLocalId(localId);



		// TEMP
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<int> dist(0, 20000);
		// TEMP

		Labels::getUnique("global").getComponent<GameManager>().addAnimal();


		entity.getComponent<Lion>().setIdleMax(20480 + dist(gen));
		entity.getComponent<Lion>().setFieldOfView(2.6f);

		// TEMP NO ONE SAW THAT
		static std::random_device rd2;
		static std::mt19937 gen2(rd2());
		static std::uniform_int_distribution<int> dist2(0, 120);
		// TEMP

		entity.getComponent<Lion>().setSpookMax(50 + dist2(gen2));


		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.02f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(0.0f, 0.0f, 0.0f));

		

		return entity;

	}













	// cow :)
	Entity& createTestHerbivoreHerd() {

		auto& entity = Core::createEntity();

		entity.addComponent<HerbivoreHerdAI>();

		entity.getComponent<HerbivoreHerdAI>().setGlobalHerdId(1);
		entity.getComponent<HerbivoreHerdAI>().setHerdLabel("cowHerd:1"); // HELLL NAH

		entity.getComponent<HerbivoreHerdAI>().setFOVRadius(3);
		entity.getComponent<HerbivoreHerdAI>().setRoamRadius(6);
		entity.getComponent<HerbivoreHerdAI>().setMigrateRadius(3);

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(entity.getComponent<HerbivoreHerdAI>().getHerdLabel());


		entity.addComponent<Transform>();
		entity.getComponent<Transform>().relocate(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

		// entity.addComponent(Light::TransformedLight{ entity.getComponent<Transform>(), 2.5f}); // test

		//const Texture texture = textureFromImage("cow.png"); // test
		//entity.addComponent(Sprite{ entity.addComponent<Transform>(), texture }); // test

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.015f);
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

		entity.addComponent<AoECow>();

		

		return entity;

	}

	Entity& createTestHerbivore(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel) {

		const Texture texture = Texture::fromImage("cow.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		entity.addComponent(Sprite{ transform, texture });
		entity.addComponent(Light::TransformedLight{ transform, 2.5f });

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel(animalLabel);
		entity.getComponent<Labels>().addLabel("cow"); //TODO

		entity.addComponent<Cow>();

		entity.addComponent<HerbivoreAI>();
		entity.getComponent<HerbivoreAI>().setHerdId(herdId);
		entity.getComponent<HerbivoreAI>().setLocalId(localId);
		entity.getComponent<HerbivoreAI>().setSpookPropagationRadius(1.0f);

		entity.getComponent<HerbivoreAI>().setHerdLabel(herdLabel);
		entity.getComponent<HerbivoreAI>().setAnimalLabel(animalLabel);


		entity.getComponent<HerbivoreAI>().addSourceOfFear("Spooky", 3.0f);

		entity.addComponent<Health>();
		entity.getComponent<Health>().setHealth(1);

		// TEMP
		static std::random_device rd;
		static std::mt19937 gen(rd());
		static std::uniform_int_distribution<int> dist(0, 20000);
		// TEMP


		entity.getComponent<HerbivoreAI>().setIdleMax(20480 + dist(gen));

		// TEMP NO ONE SAW THAT
		static std::random_device rd2;
		static std::mt19937 gen2(rd2());
		static std::uniform_int_distribution<int> dist2(0, 120);
		// TEMP

		entity.getComponent<HerbivoreAI>().setSpookMax(50 + dist2(gen2));




		entity.getComponent<HerbivoreAI>().setBaseSpeed(0.02f);

		entity.addComponent<DestinationBasedMovement>();
		//entity.getComponent<DestinationBasedMovement>().setSpeed(0.02f);

		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(0.0f, 0.0f, 0.0f));

		

		return entity;

	}

	Entity& createTreeTest(std::string path)
	{
		const Texture texture = Texture::fromImage(path);
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();
		transform.scale(3.0f);
		auto& sprite = entity.addComponent(Sprite{ transform, texture });
		entity.addComponent<Cow>();

		

		return entity;
	}


	Entity& createRandomSpookyCrap()
	{
		const Texture texture = Texture::fromImage("lioness.png");
		auto& entity = Core::createEntity();

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(5.f);

		entity.addComponent<AoECow>();

		auto& transform = entity.addComponent<Transform>();
		auto& sprite = entity.addComponent(Sprite{ transform, texture });
		entity.addComponent<Cow>();
		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel("Spooky");

		

		return entity;

	}


	/*Entity& createTestCollisionSpace() {

		auto& entity = Core::createEntity();
		auto& colSpace = entity.addComponent<AABBCollisionSpace25D>();
		colSpace.setCollisionArea(Rect(Vec2(-20.0f, -20.0f), Vec2(40.0f, 40.0f)));
		auto& labels = entity.addComponent<Labels>();
		labels.addLabel("defColSpace");

		return entity;

	}*/


	Entity& createJeep(Vec3 spawnPos)
	{
		const Texture texture = Texture::fromImage("Jeep.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		entity.addComponent(Sprite{ transform, texture });
		entity.getComponent<Transform>().relocate(spawnPos);

		entity.addComponent<Labels>();
		entity.getComponent<Labels>().addLabel("Spooky");

		entity.addComponent<Jeep>(spawnPos);
		entity.getComponent<Jeep>().setUniqueId(globalJeepId);
		globalJeepId++;

		entity.addComponent<DestinationBasedMovement>();
		entity.getComponent<DestinationBasedMovement>().setSpeed(0.01f);
		entity.getComponent<DestinationBasedMovement>().setDestination(spawnPos);

		

		return entity;
	}



	Entity& createEcosystem()
	{

		Entity& entity = Core::createEntity();
		entity.addComponent<HerdManager>();
		AnimalManager& animalManager = entity.addComponent<AnimalManager>();
		animalManager.setGlobalAnimalSpeedCoefficient(1.0f);
		animalManager.setDefIdleTimeBaseline(30.0f);
		animalManager.setDefIdleTimeFluctuation(180.0f);
		animalManager.setDefSpookPropagationRadius(1.0f);
		animalManager.setDefSpookTimeBaseline(1.0f);
		animalManager.setDefSpookTimeFluctuation(1.0f);
		FloraManager& floraManager = entity.addComponent<FloraManager>();
		Labels &labels = entity.addComponent<Labels>();
		labels.addLabel("eco");
		return entity;

	}


	Entity& createTree1()
	{
		const Texture texture = Texture::fromImage("smallTree.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();
		transform.scale(2.4f);
		auto& sprite = entity.addComponent(Sprite{ transform, texture });
		entity.addComponent<Cow>();
		return entity;

	}

	Entity& createTree2()
	{
		const Texture texture = Texture::fromImage("bigTree.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();
		transform.scale(3.4f);
		auto& sprite = entity.addComponent(Sprite{ transform, texture });
		entity.addComponent<Cow>();
		return entity;

	}

	Entity& createCactus1()
	{
		const Texture texture = Texture::fromImage("smallCactus.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();
		transform.scale(1.0f);
		auto& sprite = entity.addComponent(Sprite{ transform, texture });
		entity.addComponent<Cow>();
		return entity;

	}

	Entity& createCactus2()
	{
		const Texture texture = Texture::fromImage("mediumCactus.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();
		transform.scale(1.0f);
		auto& sprite = entity.addComponent(Sprite{ transform, texture });
		entity.addComponent<Cow>();
		return entity;

	}

	Entity& createCactus3()
	{
		const Texture texture = Texture::fromImage("bigCactus.png");
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();
		transform.scale(1.1f);
		auto& sprite = entity.addComponent(Sprite{ transform, texture });
		entity.addComponent<Cow>();
		return entity;

	}

	Entity& createSiteMarker(Vec3 position)
	{
		auto& entity = Core::createEntity();
		auto& transform = entity.addComponent<Transform>();

		const Texture texture = Texture::fromImage("testFlag.png");
		entity.addComponent(Sprite{ transform, texture });
		entity.getComponent<Transform>().relocate(position);

		entity.addComponent<FaceCamera>();

		entity.addComponent<Labels>();

		return entity;
	}


}