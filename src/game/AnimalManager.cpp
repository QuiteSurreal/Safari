#include "AnimalManager.hpp"


AnimalManager::AnimalManager() {
    globalAnimalSpeedCoefficient = 0.0f;
    defSpookPropagationRadius = 0.0f;
    defIdleTimeBaseline = 0.0f;
    defIdleTimeFluctuation = 0.0f;
    defSpookTimeBaseline = 0.0f;
    defSpookTimeFluctuation = 0.0f;
}


Entity& AnimalManager::createZebra(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>(); 
	Animator& anim = entity.addComponent<Animator>();
	HerbivoreAI& ai = entity.addComponent<HerbivoreAI>();
	Health& health = entity.addComponent<Health>();
	DestinationBasedMovement& destMov = entity.addComponent<DestinationBasedMovement>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	entity.addComponent<Selector>(1.0f);
	entity.addComponent(Light::TransformedLight{ trans, 0.0f });
	// -------

	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
    float randX = dist(gen);
	float randZ = dist(gen);

	entity.getComponent<Transform>().relocate(Vec3(randX, 0.0f, randZ));

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("zebra.png");
	entity.addComponent(Sprite{ trans, texture });

	// idle anim
	anim.addAnimation("idle", std::vector<std::string>{ "zebra.png" });

	// running anim
	std::vector<std::string> zebraRunningFrames = {
	"zebraRunning1.png",
	"zebraRunning2.png",
	"zebraRunning3.png",
	"zebraRunning4.png",
	"zebraRunning5.png",
	"zebraRunning6.png"
	};
	anim.addAnimation("running", zebraRunningFrames);

	// -------

	// --- labels ---
	labels.addLabel(animalLabel);
	labels.addLabel("zebra"); // TODO - but what?! I forgor
	labels.addLabel("animal");
	// -------

	// --- identifiers ---
	ai.setHerdId(herdId);
	ai.setLocalId(localId);
	ai.setHerdLabel(herdLabel);
	ai.setAnimalLabel(animalLabel);
	// -------
	
	// --- spook ---
	ai.setSpookPropagationRadius(defSpookPropagationRadius);
	ai.addSourceOfFear("Spooky", 3.0f);
	// -------

	// --- health ---
	health.setHealth(1);
	// -------

	// TEMP
	//static std::random_device rd;
	//static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> distIdle(0, defIdleTimeFluctuation);
	static std::uniform_int_distribution<int> distSpook(0, defSpookTimeFluctuation);
	// TEMP

	// --- behavioral settings ---
	ai.setIdleMax(defIdleTimeBaseline + distIdle(gen));
	ai.setSpookMax(defSpookTimeBaseline + distSpook(gen));
	ai.setFoodMax(2000);	
	ai.setWaterMax(2000);	
	ai.setAgingCounterMax(600.0f);
	ai.setAge(5 + (rand() % 3));
	ai.setAdultAgeFloor(4);
	ai.setRiskyAgeFloor(15);
	ai.setCertainDeathAge(20);
	ai.setBaseSpeed(globalAnimalSpeedCoefficient * 0.035f);
	ai.setSex(sex);
	destMov.setDestination(Vec3(0.0f, 0.0f, 0.0f));			
	// -------

	Labels::getUnique("global").getComponent<GameManager>().addAnimal();

	return entity;

}


Entity& AnimalManager::createWildebeest(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	Animator& anim = entity.addComponent<Animator>();
	HerbivoreAI& ai = entity.addComponent<HerbivoreAI>();
	Health& health = entity.addComponent<Health>();
	DestinationBasedMovement& destMov = entity.addComponent<DestinationBasedMovement>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	entity.addComponent<Selector>(1.0f);
	entity.addComponent(Light::TransformedLight{ trans, 0.0f });
	// -------

	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
    float randX = dist(gen);
	float randZ = dist(gen);

	entity.getComponent<Transform>().relocate(Vec3(randX, 0.0f, randZ));

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("wildebeest.png");
	entity.addComponent(Sprite{ trans, texture });

	// idle anim
	anim.addAnimation("idle", std::vector<std::string>{ "wildebeest.png" });

	// running anim
	std::vector<std::string> wildebeestRunningFrames = {
	"wildebeestRunning1.png",
	"wildebeestRunning2.png",
	"wildebeestRunning3.png",
	"wildebeestRunning4.png",
	"wildebeestRunning5.png",
	"wildebeestRunning6.png"
	};
	anim.addAnimation("running", wildebeestRunningFrames);

	// -------

	// --- labels ---
	labels.addLabel(animalLabel);
	labels.addLabel("wildebeest");
	labels.addLabel("animal");
	// -------

	// --- identifiers ---
	ai.setHerdId(herdId);
	ai.setLocalId(localId);
	ai.setHerdLabel(herdLabel);
	ai.setAnimalLabel(animalLabel);
	// -------

	// --- spook ---
	ai.setSpookPropagationRadius(defSpookPropagationRadius);
	ai.addSourceOfFear("Spooky", 3.0f);
	// -------

	// --- health ---
	health.setHealth(1);
	// -------

	// TEMP
	//static std::random_device rd;
	//static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> distIdle(0, defIdleTimeFluctuation);
	static std::uniform_int_distribution<int> distSpook(0, defSpookTimeFluctuation);
	// TEMP

	// --- behavioral settings ---
	ai.setIdleMax(defIdleTimeBaseline + distIdle(gen));
	ai.setSpookMax(defSpookTimeBaseline + distSpook(gen));
	ai.setFoodMax(2000);
	ai.setWaterMax(2000);
	ai.setAgingCounterMax(600.0f);
	ai.setAge(5 + (rand() % 3));
	ai.setAdultAgeFloor(4);
	ai.setRiskyAgeFloor(15);
	ai.setCertainDeathAge(20);
	ai.setBaseSpeed(globalAnimalSpeedCoefficient * 0.035f);
	ai.setSex(sex);
	destMov.setDestination(Vec3(0.0f, 0.0f, 0.0f));
	// -------

	Labels::getUnique("global").getComponent<GameManager>().addAnimal();

	return entity;

}


Entity& AnimalManager::createGiraffe(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	HerbivoreAI& ai = entity.addComponent<HerbivoreAI>();
	Health& health = entity.addComponent<Health>();
	DestinationBasedMovement& destMov = entity.addComponent<DestinationBasedMovement>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	// -------

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("giraffe.png");
	entity.addComponent(Sprite{ trans, texture });
	// -------

	// --- labels ---
	labels.addLabel(animalLabel);
	labels.addLabel("giraffe"); // TODO - but what?! I forgor
	// -------

	// --- identifiers ---
	ai.setHerdId(herdId);
	ai.setLocalId(localId);
	ai.setHerdLabel(herdLabel);
	ai.setAnimalLabel(animalLabel);
	// -------

	// --- spook ---
	ai.setSpookPropagationRadius(defSpookPropagationRadius);
	ai.addSourceOfFear("Spooky", 3.0f);
	// -------

	// --- health ---
	health.setHealth(1);
	// -------

	// TEMP
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> distIdle(0, defIdleTimeFluctuation);
	static std::uniform_int_distribution<int> distSpook(0, defSpookTimeFluctuation);
	// TEMP

	// --- behavioral settings ---
	ai.setIdleMax(defIdleTimeBaseline + distIdle(gen));
	ai.setSpookMax(defSpookTimeBaseline + distSpook(gen));
	ai.setBaseSpeed(globalAnimalSpeedCoefficient * 0.02f);
	ai.setSex(sex);
	destMov.setDestination(Vec3(0.0f, 0.0f, 0.0f));			// TEMP
	// -------

	Labels::getUnique("global").getComponent<GameManager>().addAnimal();

	return entity;

}


Entity& AnimalManager::createRhino(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	HerbivoreAI& ai = entity.addComponent<HerbivoreAI>();
	Health& health = entity.addComponent<Health>();
	DestinationBasedMovement& destMov = entity.addComponent<DestinationBasedMovement>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	// -------

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("rhino.png");
	entity.addComponent(Sprite{ trans, texture });
	// -------

	// --- labels ---
	labels.addLabel(animalLabel);
	labels.addLabel("rhino"); // TODO - but what?! I forgor
	// -------

	// --- identifiers ---
	ai.setHerdId(herdId);
	ai.setLocalId(localId);
	ai.setHerdLabel(herdLabel);
	ai.setAnimalLabel(animalLabel);
	// -------

	// --- spook ---
	ai.setSpookPropagationRadius(defSpookPropagationRadius);
	ai.addSourceOfFear("Spooky", 3.0f);
	// -------

	// --- health ---
	health.setHealth(1);
	// -------

	// TEMP
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> distIdle(0, defIdleTimeFluctuation);
	static std::uniform_int_distribution<int> distSpook(0, defSpookTimeFluctuation);
	// TEMP

	// --- behavioral settings ---
	ai.setIdleMax(defIdleTimeBaseline + distIdle(gen));
	ai.setSpookMax(defSpookTimeBaseline + distSpook(gen));
	ai.setBaseSpeed(globalAnimalSpeedCoefficient * 0.02f);
	ai.setSex(sex);
	destMov.setDestination(Vec3(0.0f, 0.0f, 0.0f));			// TEMP
	// -------

	return entity;

}

/*
void AnimalManager::createLion(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex) {

}


void AnimalManager::createHyena(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex) {

}
*/

Entity& AnimalManager::createLoveMarker(const Vec3& pos) {

	Entity& entity = Core::createEntity();
	Transform& trans = entity.addComponent<Transform>();
	entity.addComponent<FaceCamera>();

	trans.relocate(pos + Vec3(0.0f,1.0f,0.0f));

	const Texture texture = Texture::fromImage("heartTemp.png");
	entity.addComponent(Sprite{ trans, texture });

	return entity;

}

Entity& AnimalManager::createLoveMarker(Transform& reference) {

	Entity& entity = Core::createEntity();
	Transform& trans = entity.addComponent<Transform>();
	entity.addComponent<FaceCamera>();

	trans.setParent(reference);
	trans.translate(Vec3(0.0f, 1.0f, 0.0f));

	const Texture texture = Texture::fromImage("heartTemp.png");
	entity.addComponent(Sprite{ trans, texture });

	return entity;

}





Entity& AnimalManager::createLion(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	Animator& anim = entity.addComponent<Animator>();
	CarnivoreAI& ai = entity.addComponent<CarnivoreAI>();
	Health& health = entity.addComponent<Health>();
	DestinationBasedMovement& destMov = entity.addComponent<DestinationBasedMovement>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	entity.addComponent<Selector>(1.0f);
	entity.addComponent(Light::TransformedLight{ trans, 0.0f });
	// -------

	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
    float randX = dist(gen);
	float randZ = dist(gen);

	entity.getComponent<Transform>().relocate(Vec3(randX, 0.0f, randZ));

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("lion.png");
	entity.addComponent(Sprite{ trans, texture });

	// idle anim
	anim.addAnimation("idle", std::vector<std::string>{ "lion.png" });

	// running anim
	std::vector<std::string> lionRunningFrames = {
	"lionRunning1.png",
	"lionRunning2.png",
	"lionRunning3.png",
	"lionRunning4.png",
	"lionRunning5.png"
	};
	anim.addAnimation("running", lionRunningFrames);

	// -------

	// --- labels ---
	labels.addLabel(animalLabel);
	labels.addLabel("lion");
	labels.addLabel("animal");
	// -------

	// --- identifiers ---
	ai.setHerdId(herdId);
	ai.setLocalId(localId);
	ai.setHerdLabel(herdLabel);
	ai.setAnimalLabel(animalLabel);
	// -------

	// --- spook ---
	ai.setSpookPropagationRadius(defSpookPropagationRadius);
	ai.addSourceOfFear("EVA", 3.0f);
	// -------

	// --- health ---
	health.setHealth(1);
	// -------

	// TEMP
	//static std::random_device rd;
	//static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> distIdle(0, defIdleTimeFluctuation);
	static std::uniform_int_distribution<int> distSpook(0, defSpookTimeFluctuation);
	// TEMP

	// --- behavioral settings ---
	ai.setIdleMax(defIdleTimeBaseline + distIdle(gen));
	ai.setSpookMax(defSpookTimeBaseline + distSpook(gen));
	ai.setFoodMax(8000);
	ai.setWaterMax(8000);
	ai.setAgingCounterMax(600.0f);
	ai.setAge(5 + (rand() % 3));
	ai.setAdultAgeFloor(4);
	ai.setRiskyAgeFloor(15);
	ai.setCertainDeathAge(20);
	ai.setBaseSpeed(globalAnimalSpeedCoefficient * 0.035f);
	ai.setSex(sex);
	destMov.setDestination(Vec3(0.0f, 0.0f, 0.0f));
	// -------

	return entity;

}






Entity& AnimalManager::createHyena(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	Animator& anim = entity.addComponent<Animator>();
	CarnivoreAI& ai = entity.addComponent<CarnivoreAI>();
	Health& health = entity.addComponent<Health>();
	DestinationBasedMovement& destMov = entity.addComponent<DestinationBasedMovement>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	entity.addComponent<Selector>(1.0f);
	entity.addComponent(Light::TransformedLight{ trans, 0.0f });
	// -------

	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-10.0f, 10.0f);
    float randX = dist(gen);
	float randZ = dist(gen);

	entity.getComponent<Transform>().relocate(Vec3(randX, 0.0f, randZ));

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("hyena.png");
	entity.addComponent(Sprite{ trans, texture });

	// idle anim
	anim.addAnimation("idle", std::vector<std::string>{ "hyena.png" });

	// running anim
	std::vector<std::string> hyenaRunningFrames = {
	"hyenaRunning1.png",
	"hyenaRunning2.png",
	"hyenaRunning3.png",
	"hyenaRunning4.png",
	"hyenaRunning5.png"
	};
	anim.addAnimation("running", hyenaRunningFrames);

	// -------

	// --- labels ---
	labels.addLabel(animalLabel);
	labels.addLabel("hyena");
	labels.addLabel("animal");
	// -------

	// --- identifiers ---
	ai.setHerdId(herdId);
	ai.setLocalId(localId);
	ai.setHerdLabel(herdLabel);
	ai.setAnimalLabel(animalLabel);
	// -------

	// --- spook ---
	ai.setSpookPropagationRadius(defSpookPropagationRadius);
	ai.addSourceOfFear("EVA", 3.0f);
	// -------

	// --- health ---
	health.setHealth(1);
	// -------

	// TEMP
	//static std::random_device rd;
	//static std::mt19937 gen(rd());
	static std::uniform_int_distribution<int> distIdle(0, defIdleTimeFluctuation);
	static std::uniform_int_distribution<int> distSpook(0, defSpookTimeFluctuation);
	// TEMP

	// --- behavioral settings ---
	ai.setIdleMax(defIdleTimeBaseline + distIdle(gen));
	ai.setSpookMax(defSpookTimeBaseline + distSpook(gen));
	ai.setFoodMax(8000);
	ai.setWaterMax(8000);
	ai.setAgingCounterMax(600.0f);
	ai.setAge(5 + (rand() % 3));
	ai.setAdultAgeFloor(4);
	ai.setRiskyAgeFloor(15);
	ai.setCertainDeathAge(20);
	ai.setBaseSpeed(globalAnimalSpeedCoefficient * 0.035f);
	ai.setSex(sex);
	destMov.setDestination(Vec3(0.0f, 0.0f, 0.0f));
	// -------

	return entity;

}