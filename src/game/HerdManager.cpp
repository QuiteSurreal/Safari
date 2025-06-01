#include "HerdManager.hpp"


HerdManager::HerdManager() {}


void HerdManager::createZebraHerd(Vec3 startPos, int initialSize) {

	auto& entity = Core::createEntity();

	int globalId = HerdManager::getNextHerdId(HerdTypes::Zebra);

	HerbivoreHerdAI& ai = entity.addComponent<HerbivoreHerdAI>();
    ai.setType(HerdTypes::Zebra);
	ai.setGlobalHerdId(globalId);
	ai.setHerdLabel("zebraHerd:" + std::to_string(globalId));
	ai.setFOVRadius(3);         
	ai.setRoamRadius(6);		
	ai.setMigrateRadius(3);		
    ai.setMaxHerdSize(12);
    ai.setMaxReproductionCounter(16800);
    ai.setSatietyComfortLimit(0.5f);
    ai.setSatietyCriticalLimit(0.2f);
    ai.setHydrationComfortLimit(0.5f);
    ai.setHydrationCriticalLimit(0.2f);

    HerbivoreSiteMemory& siteMemory = entity.addComponent<HerbivoreSiteMemory>();
    siteMemory.setCapacity(100);
    siteMemory.setMinSiteDistance(0.2f);
    siteMemory.setSiteRadius(6);
    siteMemory.addPlantPreference("normalGrass", 5);
    siteMemory.addPlantPreference("thickGrass", 1);
    siteMemory.addPlantPreference("redGrass", 2);
    siteMemory.addPlantPreference("exoticGrass", 7);

	Labels& labels = entity.addComponent<Labels>();
	labels.addLabel(ai.getHerdLabel());

	Transform& trans = entity.addComponent<Transform>();
	trans.relocate(startPos);

	DestinationBasedMovement& destMov = entity.addComponent<DestinationBasedMovement>();
    destMov.setSpeed(0.025f);
	destMov.setDestination(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

	for (int i = 0; i < initialSize; i++) {
		auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
		getEntity().getComponent<AnimalManager>().createZebra(herdId, localId, herdLabel, animalLabel, Sexes::Male);
	}

    auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
    getEntity().getComponent<AnimalManager>().createZebra(herdId, localId, herdLabel, animalLabel, Sexes::Female);

    herdMap[HerdTypes::Zebra].push_back(&entity);

}


void HerdManager::createWildebeestHerd(Vec3 startPos, int initialSize) {

    auto& entity = Core::createEntity();

    int globalId = HerdManager::getNextHerdId(HerdTypes::Wildebeest);

    HerbivoreHerdAI& ai = entity.addComponent<HerbivoreHerdAI>();
    ai.setType(HerdTypes::Wildebeest);
    ai.setGlobalHerdId(globalId);
    ai.setHerdLabel("wildebeestHerd:" + std::to_string(globalId));
    ai.setFOVRadius(3);
    ai.setRoamRadius(6);
    ai.setMigrateRadius(3);
    ai.setMaxHerdSize(12);
    ai.setMaxReproductionCounter(16800);
    ai.setSatietyComfortLimit(0.5f);
    ai.setSatietyCriticalLimit(0.2f);
    ai.setHydrationComfortLimit(0.5f);
    ai.setHydrationCriticalLimit(0.2f);

    HerbivoreSiteMemory& siteMemory = entity.addComponent<HerbivoreSiteMemory>();
    siteMemory.setCapacity(100);
    siteMemory.setMinSiteDistance(0.2f);
    siteMemory.setSiteRadius(6);
    siteMemory.addPlantPreference("normalGrass", 3);
    siteMemory.addPlantPreference("thickGrass", 6);
    siteMemory.addPlantPreference("redGrass", 6);
    siteMemory.addPlantPreference("exoticGrass", 8);

    Labels& labels = entity.addComponent<Labels>();
    labels.addLabel(ai.getHerdLabel());

    Transform& trans = entity.addComponent<Transform>();
    trans.relocate(startPos);

    DestinationBasedMovement& destMov = entity.addComponent<DestinationBasedMovement>();
    destMov.setSpeed(0.025f);
    destMov.setDestination(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

    for (int i = 0; i < initialSize; i++) {
        auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
        getEntity().getComponent<AnimalManager>().createWildebeest(herdId, localId, herdLabel, animalLabel, Sexes::Female);
    }

    auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
    getEntity().getComponent<AnimalManager>().createWildebeest(herdId, localId, herdLabel, animalLabel, Sexes::Male);

    herdMap[HerdTypes::Wildebeest].push_back(&entity);

}

/*
void HerdManager::createGiraffeHerd(Vec3 startPos) {


}


void HerdManager::createRhinoHerd(Vec3 startPos) {


}


void HerdManager::createLionPride(Vec3 startPos) {


}

void HerdManager::createHyenaPack(Vec3 startPos) {


}*/


void HerdManager::createLionPride(Vec3 startPos, int initialSize) {

    auto& entity = Core::createEntity();

    int globalId = HerdManager::getNextHerdId(HerdTypes::Lion);

    CarnivorePackAI& ai = entity.addComponent<CarnivorePackAI>();
    ai.setType(HerdTypes::Lion);
    ai.setGlobalHerdId(globalId);
    ai.setHerdLabel("lionPride:" + std::to_string(globalId));
    ai.setFOVRadius(3);
    ai.setRoamRadius(6);
    ai.setMigrateRadius(3);
    ai.setMaxHerdSize(6);
    ai.setMaxReproductionCounter(16800);
    ai.setSatietyComfortLimit(0.5f);
    ai.setSatietyCriticalLimit(0.2f);
    ai.setHydrationComfortLimit(0.5f);
    ai.setHydrationCriticalLimit(0.2f);

    Labels& labels = entity.addComponent<Labels>();
    labels.addLabel(ai.getHerdLabel());

    Transform& trans = entity.addComponent<Transform>();
    trans.relocate(startPos);

    DestinationBasedMovement& destMov = entity.addComponent<DestinationBasedMovement>();
    destMov.setSpeed(0.04f);
    destMov.setDestination(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

    for (int i = 0; i < initialSize; i++) {
        auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
        getEntity().getComponent<AnimalManager>().createLion(herdId, localId, herdLabel, animalLabel, Sexes::Female);
    }

    auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
    getEntity().getComponent<AnimalManager>().createLion(herdId, localId, herdLabel, animalLabel, Sexes::Male);

    herdMap[HerdTypes::Lion].push_back(&entity);

}




void HerdManager::createHyenaPack(Vec3 startPos, int initialSize) {

    auto& entity = Core::createEntity();

    int globalId = HerdManager::getNextHerdId(HerdTypes::Hyena);

    CarnivorePackAI& ai = entity.addComponent<CarnivorePackAI>();
    ai.setType(HerdTypes::Hyena);
    ai.setGlobalHerdId(globalId);
    ai.setHerdLabel("hyenaPack:" + std::to_string(globalId));
    ai.setFOVRadius(3);
    ai.setRoamRadius(6);
    ai.setMigrateRadius(3);
    ai.setMaxHerdSize(6);
    ai.setMaxReproductionCounter(16800);
    ai.setSatietyComfortLimit(0.5f);
    ai.setSatietyCriticalLimit(0.2f);
    ai.setHydrationComfortLimit(0.5f);
    ai.setHydrationCriticalLimit(0.2f);

    Labels& labels = entity.addComponent<Labels>();
    labels.addLabel(ai.getHerdLabel());

    Transform& trans = entity.addComponent<Transform>();
    trans.relocate(startPos);

    DestinationBasedMovement& destMov = entity.addComponent<DestinationBasedMovement>();
    destMov.setSpeed(0.04f);
    destMov.setDestination(Vec3(randf(-20.0f, 20.0f), 0.0f, randf(-20.0f, 20.0f)));

    for (int i = 0; i < initialSize; i++) {
        auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
        getEntity().getComponent<AnimalManager>().createHyena(herdId, localId, herdLabel, animalLabel, Sexes::Female);
    }

    auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
    getEntity().getComponent<AnimalManager>().createHyena(herdId, localId, herdLabel, animalLabel, Sexes::Male);

    herdMap[HerdTypes::Hyena].push_back(&entity);

}




Entity& HerdManager::addAnimalToHerd(HerdTypes type, Sexes sex, int id) {

    // Check if the id is valid.
    const auto& herd = herdMap.at(type);
    if (id < 0 || id >= static_cast<int>(herd.size())) {
        throw std::out_of_range("Invalid animal id for the herd.");
    }

    Entity* herdEntity = herd[id];
    Entity* animalEntity;

    // Ugly, but we are limited by the component based solution.
    switch (type) {
    case HerdTypes::Zebra:
    {
        HerbivoreHerdAI& ai = herdEntity->getComponent<HerbivoreHerdAI>();
        auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
        animalEntity = &(getEntity().getComponent<AnimalManager>().createZebra(herdId, localId, herdLabel, animalLabel, sex));
        break;
    }
    case HerdTypes::Wildebeest:
    {
        HerbivoreHerdAI& ai = herdEntity->getComponent<HerbivoreHerdAI>();
        auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
        // TODO
        animalEntity = &(getEntity().getComponent<AnimalManager>().createWildebeest(herdId, localId, herdLabel, animalLabel, sex));
        break;
    }
    case HerdTypes::Lion:
    {
        CarnivorePackAI& ai = herdEntity->getComponent<CarnivorePackAI>();
        auto [herdId, localId, herdLabel, animalLabel] = ai.addAnimal();
        // TODO
        animalEntity = &(getEntity().getComponent<AnimalManager>().createLion(herdId, localId, herdLabel, animalLabel, sex));
        break;
    }
    case HerdTypes::Hyena:
        // waiting for carnivoreAI

    default:
        throw std::runtime_error("Unknown herd type.");
    }

    return *animalEntity;
}



int HerdManager::getNextHerdId(HerdTypes type) {

    std::size_t count = 0;
    if (auto it = herdMap.find(type); it != herdMap.end())
        count = it->second.size();          

    return count;

}
