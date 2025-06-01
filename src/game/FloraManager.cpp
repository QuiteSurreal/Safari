#include "FloraManager.hpp"

FloraManager::FloraManager() {
	initialPlantCounts = {
		{ EdiblePlantTypes::NormalGrass, 0 },
		{ EdiblePlantTypes::ThickGrass, 0 },
		{ EdiblePlantTypes::RedGrass, 0 },
		{ EdiblePlantTypes::ExoticGrass, 0 },
		{ EdiblePlantTypes::EdibleTree, 0 }
	};
}

Entity& FloraManager::createNormalGrass(Vec3 startPos) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	EdiblePlant& behavior = entity.addComponent<EdiblePlant>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	// -------

	// --- plants ---
	behavior.setMaxFoodValue(400);
	behavior.setMaxGrowthCounter(10);
	behavior.setConsumableLimit(370);
	// -------

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("normalGrassFull1.png");
	Sprite& sprite = entity.addComponent(Sprite{ trans, texture });
	Animator& anim = entity.addComponent<Animator>();

	// TEMP

	std::vector<std::string> intact = {
	"normalGrassFull1.png",
	"normalGrassFull2.png",
	"normalGrassFull3.png",
	"normalGrassFull2.png"
	};

	std::vector<std::string> half = {
	"normalGrassHalf1.png",
	"normalGrassHalf2.png",
	"normalGrassHalf3.png",
	"normalGrassHalf2.png"
	};

	std::vector<std::string> scarce = {
	"normalGrassScarce1.png",
	"normalGrassScarce2.png",
	"normalGrassScarce3.png",
	"normalGrassScarce2.png"
	};

	std::vector<std::string> depleted = {
	"normalGrassDepleted.png"
	};

	anim.addAnimation("intact", intact);
	anim.addAnimation("half", half);
	anim.addAnimation("scarce", scarce);
	anim.addAnimation("depleted", depleted);
	anim.setActive(true);
	int plus = static_cast<int>(randf(1,6));
	anim.setDelay(14 + plus);
	anim.switchAnimation("intact");
	// TEMP

	// -------

	// --- labels ---
	labels.addLabel("normalGrass");
	labels.addLabel("ediblePlant");
	// -------

	// --- other ---
	trans.relocate(startPos);
	trans.scale(0.8);
	// -------

	plantMap[EdiblePlantTypes::NormalGrass].push_back(&entity);
	return entity;

}

Entity& FloraManager::createThickGrass(Vec3 startPos) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	EdiblePlant& behavior = entity.addComponent<EdiblePlant>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	// -------

	// --- plants ---
	behavior.setMaxFoodValue(200);
	behavior.setMaxGrowthCounter(200);
	behavior.setConsumableLimit(180);
	// -------

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("thickGrassFull1.png");
	entity.addComponent(Sprite{ trans, texture });
	Animator& anim = entity.addComponent<Animator>();

	// TEMP

	std::vector<std::string> intact = {
	"thickGrassFull1.png",
	"thickGrassFull2.png",
	"thickGrassFull3.png",
	"thickGrassFull2.png"
	};

	std::vector<std::string> half = {
	"thickGrassFull1.png",
	"thickGrassFull2.png",
	"thickGrassFull3.png",
	"thickGrassFull2.png"
	};

	std::vector<std::string> scarce = {
	"thickGrassHalf1.png",
	"thickGrassHalf2.png",
	"thickGrassHalf3.png",
	"thickGrassHalf2.png"
	};

	std::vector<std::string> depleted = {
	"thickGrassDepleted.png",
	"thickGrassDepleted.png",
	"thickGrassDepleted.png",
	"thickGrassDepleted.png"
	};

	anim.addAnimation("intact", intact);
	anim.addAnimation("half", half);
	anim.addAnimation("scarce", scarce);
	anim.addAnimation("depleted", depleted);
	anim.setActive(true);
	int plus = static_cast<int>(randf(1, 6));
	anim.setDelay(14 + plus);
	anim.switchAnimation("intact");
	// TEMP
	
	// -------

	// --- labels ---
	labels.addLabel("thickGrass");
	labels.addLabel("ediblePlant");
	// -------

	// --- other ---
	trans.relocate(startPos);
	// -------

	plantMap[EdiblePlantTypes::ThickGrass].push_back(&entity);
	return entity;

}

Entity& FloraManager::createRedGrass(Vec3 startPos) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	EdiblePlant& behavior = entity.addComponent<EdiblePlant>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	// -------

	// --- plants ---
	behavior.setMaxFoodValue(200);
	behavior.setMaxGrowthCounter(200);
	behavior.setConsumableLimit(180);
	// -------

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("redGrass.png");
	entity.addComponent(Sprite{ trans, texture });
	Animator& anim = entity.addComponent<Animator>();

	// TEMP

	std::vector<std::string> intact = {
	"redGrassFull1.png",
	"redGrassFull2.png",
	"redGrassFull3.png",
	"redGrassFull2.png"
	};

	std::vector<std::string> half = {
	"redGrassFull1.png",
	"redGrassFull2.png",
	"redGrassFull3.png",
	"redGrassFull2.png"
	};

	std::vector<std::string> scarce = {
	"redGrassHalf.png",
	"redGrassHalf.png",
	"redGrassHalf.png",
	"redGrassHalf.png"
	};

	std::vector<std::string> depleted = {
	"redGrassDepleted.png",
	"redGrassDepleted.png",
	"redGrassDepleted.png",
	"redGrassDepleted.png"
	};

	anim.addAnimation("intact", intact);
	anim.addAnimation("half", half);
	anim.addAnimation("scarce", scarce);
	anim.addAnimation("depleted", depleted);
	anim.setActive(true);
	int plus = static_cast<int>(randf(1, 6));
	anim.setDelay(14 + plus);
	anim.switchAnimation("intact");
	// TEMP

	// -------

	// --- labels ---
	labels.addLabel("redGrass");
	labels.addLabel("ediblePlant");
	// -------

	// --- other ---
	trans.relocate(startPos);
	// -------

	plantMap[EdiblePlantTypes::RedGrass].push_back(&entity);
	return entity;

}

Entity& FloraManager::createExoticGrass(Vec3 startPos) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	EdiblePlant& behavior = entity.addComponent<EdiblePlant>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	// -------

	// --- plants ---
	behavior.setMaxFoodValue(200);
	behavior.setMaxGrowthCounter(200);
	behavior.setConsumableLimit(180);
	// -------

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("exoticGrass.png");
	entity.addComponent(Sprite{ trans, texture });
	Animator& anim = entity.addComponent<Animator>();

	// TEMP

	std::vector<std::string> intact = {
	"exoticGrassFull.png",
	"exoticGrassFull.png",
	"exoticGrassFull.png",
	"exoticGrassFull.png"
	};

	std::vector<std::string> half = {
	"exoticGrassFull.png",
	"exoticGrassFull.png",
	"exoticGrassFull.png",
	"exoticGrassFull.png"
	};

	std::vector<std::string> scarce = {
	"exoticGrassHalf.png",
	"exoticGrassHalf.png",
	"exoticGrassHalf.png",
	"exoticGrassHalf.png"
	};

	std::vector<std::string> depleted = {
	"exoticGrassDepleted.png",
	"exoticGrassDepleted.png",
	"exoticGrassDepleted.png",
	"exoticGrassDepleted.png"
	};

	anim.addAnimation("intact", intact);
	anim.addAnimation("half", half);
	anim.addAnimation("scarce", scarce);
	anim.addAnimation("depleted", depleted);
	anim.setActive(true);
	int plus = static_cast<int>(randf(1, 6));
	anim.setDelay(14 + plus);
	anim.switchAnimation("intact");
	// TEMP

	// -------

	// --- labels ---
	labels.addLabel("exoticGrass");
	labels.addLabel("ediblePlant");
	// -------

	// --- other ---
	trans.relocate(startPos);
	// -------

	plantMap[EdiblePlantTypes::ExoticGrass].push_back(&entity);
	return entity;

}

Entity& FloraManager::createEdibleTree(Vec3 startPos) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	EdiblePlant& behavior = entity.addComponent<EdiblePlant>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	// -------

	// --- plants ---
	behavior.setMaxFoodValue(200);
	behavior.setMaxGrowthCounter(200);
	behavior.setConsumableLimit(180);
	// -------

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("edibleTree.png");
	entity.addComponent(Sprite{ trans, texture });
	// -------

	// --- labels ---
	labels.addLabel("edibleTree");
	labels.addLabel("ediblePlant");
	// -------

	// --- other ---
	trans.relocate(startPos);
	// -------

	plantMap[EdiblePlantTypes::EdibleTree].push_back(&entity);
	return entity;

}


Entity& FloraManager::createReed1(Vec3 startPos) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	EdiblePlant& behavior = entity.addComponent<EdiblePlant>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	// -------

	// --- plants ---
	behavior.setMaxFoodValue(200);
	behavior.setMaxGrowthCounter(200);
	behavior.setConsumableLimit(180);
	// -------

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("reeda1.png");
	entity.addComponent(Sprite{ trans, texture });
	Animator& anim = entity.addComponent<Animator>();

	// TEMP

	std::vector<std::string> intact = {
	"reeda1.png",
	"reeda2.png",
	"reeda3.png",
	"reeda2.png"
	};

	std::vector<std::string> half = {
	"reeda1.png",
	"reeda2.png",
	"reeda3.png",
	"reeda2.png"
	};

	std::vector<std::string> scarce = {
	"reeda1.png",
	"reeda2.png",
	"reeda3.png",
	"reeda2.png"
	};

	std::vector<std::string> depleted = {
	"reeda1.png",
	"reeda2.png",
	"reeda3.png",
	"reeda2.png"
	};

	anim.addAnimation("intact", intact);
	anim.addAnimation("half", half);
	anim.addAnimation("scarce", scarce);
	anim.addAnimation("depleted", depleted);
	anim.setActive(true);
	int plus = static_cast<int>(randf(1, 6));
	anim.setDelay(14 + plus);
	anim.switchAnimation("intact");
	// TEMP

	// -------

	// --- labels ---
	labels.addLabel("reeda");
	labels.addLabel("ediblePlant");
	// -------

	// --- other ---
	trans.relocate(startPos);
	// -------

	plantMap[EdiblePlantTypes::ExoticGrass].push_back(&entity);
	return entity;

}


Entity& FloraManager::createReed2(Vec3 startPos) {

	auto& entity = Core::createEntity();

	// --- components ---
	auto& trans = entity.addComponent<Transform>();
	Labels& labels = entity.addComponent<Labels>();
	entity.addComponent<FaceCamera>();
	EdiblePlant& behavior = entity.addComponent<EdiblePlant>();
	Targetable& targ = entity.addComponent<Targetable>();
	targ.setTargetable(true);
	// -------

	// --- plants ---
	behavior.setMaxFoodValue(200);
	behavior.setMaxGrowthCounter(200);
	behavior.setConsumableLimit(180);
	// -------

	// --- sprite and animation ---
	const Texture texture = Texture::fromImage("reedb1.png");
	entity.addComponent(Sprite{ trans, texture });
	Animator& anim = entity.addComponent<Animator>();

	// TEMP

	std::vector<std::string> intact = {
	"reedb1.png",
	"reedb2.png",
	"reedb3.png",
	"reedb2.png"
	};

	std::vector<std::string> half = {
	"reedb1.png",
	"reedb2.png",
	"reedb3.png",
	"reedb2.png"
	};

	std::vector<std::string> scarce = {
	"reedb1.png",
	"reedb2.png",
	"reedb3.png",
	"reedb2.png"
	};

	std::vector<std::string> depleted = {
	"reedb1.png",
	"reedb2.png",
	"reedb3.png",
	"reedb2.png"
	};

	anim.addAnimation("intact", intact);
	anim.addAnimation("half", half);
	anim.addAnimation("scarce", scarce);
	anim.addAnimation("depleted", depleted);
	anim.setActive(true);
	int plus = static_cast<int>(randf(1, 6));
	anim.setDelay(14 + plus);
	anim.switchAnimation("intact");
	// TEMP

	// -------

	// --- labels ---
	labels.addLabel("reedb");
	labels.addLabel("ediblePlant");
	// -------

	// --- other ---
	trans.relocate(startPos);
	// -------

	plantMap[EdiblePlantTypes::ExoticGrass].push_back(&entity);
	return entity;

}


void FloraManager::setInitialPlantCount(EdiblePlantTypes plantType, int count) {
	initialPlantCounts[plantType] = count;
}

int FloraManager::getInitialPlantCount(EdiblePlantTypes plantType) const {

	auto it = initialPlantCounts.find(plantType);
	if (it != initialPlantCounts.end()) {
		return it->second;
	}
	return 0;

}

void FloraManager::generateInitialPlants(float clumpiness) {

	// Choosing the water tiles
	Entity& baseGrid = Labels::getUnique("global");
	Grid& grid = baseGrid.getComponent<Grid>();
	std::vector<Ivec2> waterTiles = grid.getAllOfType("water");
	float tileSize = grid.getTileSize();
	Ivec2 gridSize = grid.getGridSize();
	Vec3 gridOrigin = grid.getPosition();
	// ----------------------------

	// DEBUG 
	std::string str = std::to_string(waterTiles.size());
	logDebug(str.c_str());
	//

	// Choosing the valid tiles
	std::vector<Ivec2> nearbyTiles;

	for (const Ivec2& waterTile : waterTiles)
	{
		Vec3 waterPos = gridOrigin + Vec3(waterTile.x * tileSize, 0.0f, waterTile.y * tileSize);

		for (int x = 0; x < gridSize.x; ++x)
		{
			for (int y = 0; y < gridSize.y; ++y)
			{
				Ivec2 currentTile = { x, y };
				Vec3 currentPos = gridOrigin + Vec3(x * tileSize, 0.0f, y * tileSize);

				float distance = glm::distance(waterPos, currentPos);
				if (distance <= 6.0f && grid.getType(Ivec2(x,y)) != "water" && grid.getType(Ivec2(x, y)) != "road")
				{
					nearbyTiles.push_back(currentTile);
				}
			}
		}
	}


	for (const auto& [plantType, count] : initialPlantCounts) {

		if (count <= 0)
			continue;

		// Group count
		int groupCount = static_cast<int>(std::round(count * (1.0f - clumpiness) + 1.0f * clumpiness));
		groupCount = std::max(1, groupCount);

		// Group radius
		float groupRadius = 40.0f * clumpiness * 0.02f; // TWO MAGIC NUMBERS, clear them later

		// Group centers 
		std::vector<Vec3> groupCenters;

		if (waterTiles.empty()) {
			logDebug("No tiles near water - which is impossible.");
			continue;
		}

		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> tilePicker(0, static_cast<int>(nearbyTiles.size()) - 1);

		for (int i = 0; i < groupCount; ++i) {

			Ivec2 tile = nearbyTiles[tilePicker(rng)];

			float x = randf(tile.x * tileSize, (tile.x + 1) * tileSize);
			float z = randf(tile.y * tileSize, (tile.y + 1) * tileSize);

			Vec3 groupCenter = gridOrigin + Vec3(x, 0.0f, z);

			groupCenters.push_back(groupCenter);
		}

		// Group distributions --------
		std::vector<float> groupWeights(groupCount);
		float totalWeight = 0.0f;
		std::vector<int> plantsPerGroup(groupCount);

		// Weights 
		for (int i = 0; i < groupCount; ++i) {
			groupWeights[i] = randf(0.5f, 1.5f);  // MAGIC NUMBERS HERE TOO: we could use a parameter for weight variation
			totalWeight += groupWeights[i];
		}

		// Normalization of the weights
		for (int i = 0; i < groupCount; ++i) {
			groupWeights[i] /= totalWeight;
		}

		// Assign plant counts per group
		int assignedPlants = 0;

		for (int i = 0; i < groupCount; ++i) {
			plantsPerGroup[i] = static_cast<int>(std::round(groupWeights[i] * count));
			assignedPlants += plantsPerGroup[i];
		}

		// Fix rounding errors
		while (assignedPlants > count) {
			for (int i = 0; i < groupCount && assignedPlants > count; ++i) {
				if (plantsPerGroup[i] > 0) {
					plantsPerGroup[i] -= 1;
					assignedPlants -= 1;
				}
			}
		}

		while (assignedPlants < count) {
			for (int i = 0; i < groupCount && assignedPlants < count; ++i) {
				plantsPerGroup[i] += 1;
				assignedPlants += 1;
			}
		}

		// Generate the plants ---

		for (int groupIndex = 0; groupIndex < groupCount; ++groupIndex) {

			Vec3 groupCenter = groupCenters[groupIndex];
			float adjustedRadius = groupRadius * groupWeights[groupIndex];
			int plantCount = plantsPerGroup[groupIndex];

			// Collect valid tiles in range
			std::vector<Ivec2> validTilesInRange;

			for (const Ivec2& tile : nearbyTiles) {

				Vec3 tileCenter = gridOrigin + Vec3(
					tile.x * tileSize + tileSize * 0.5f,
					0.0f,
					tile.y * tileSize + tileSize * 0.5f
				);

				float distance = glm::distance(groupCenter, tileCenter);

				if (distance <= adjustedRadius + tileSize * 0.5f) {
					validTilesInRange.push_back(tile);
				}
			}

			if (validTilesInRange.empty())
				continue; 

			std::uniform_int_distribution<int> tilePicker(0, static_cast<int>(validTilesInRange.size()) - 1);

			// Spawn plants randomly in valid tiles

			for (int p = 0; p < plantCount; ++p) {

				Ivec2 tile = validTilesInRange[tilePicker(rng)];

				float x = randf(tile.x * tileSize, (tile.x + 1) * tileSize);
				float z = randf(tile.y * tileSize, (tile.y + 1) * tileSize);

				Vec3 plantPosition = gridOrigin + Vec3(x, 0.0f, z);

				switch (plantType) {

				case EdiblePlantTypes::NormalGrass:
					createNormalGrass(plantPosition);
					break;

				case EdiblePlantTypes::ThickGrass:
					createThickGrass(plantPosition);
					break;

				case EdiblePlantTypes::RedGrass:
					createRedGrass(plantPosition);
					break;

				case EdiblePlantTypes::ExoticGrass:
					createExoticGrass(plantPosition);
					break;

				case EdiblePlantTypes::EdibleTree:
					createEdibleTree(plantPosition);
					break;

				default:
					break;
				}

			}
		}

	}

}




void FloraManager::generateInitialWaterPlants(float clumpiness, const std::unordered_map<EdiblePlantTypes, int>& waterPlants) {

	// Choosing the water tiles
	Entity& baseGrid = Labels::getUnique("global");
	Grid& grid = baseGrid.getComponent<Grid>();
	std::vector<Ivec2> waterTiles = grid.getAllOfType("water");
	float tileSize = grid.getTileSize();
	Ivec2 gridSize = grid.getGridSize();
	Vec3 gridOrigin = grid.getPosition();
	// ----------------------------

	// DEBUG 
	std::string str = std::to_string(waterTiles.size());
	logDebug(str.c_str());
	//

	// Choosing the valid tiles
	std::vector<Ivec2> nearbyTiles;

	for (const Ivec2& waterTile : waterTiles)
	{
		Vec3 waterPos = gridOrigin + Vec3(waterTile.x * tileSize, 0.0f, waterTile.y * tileSize);

		for (int x = 0; x < gridSize.x; ++x)
		{
			for (int y = 0; y < gridSize.y; ++y)
			{
				Ivec2 currentTile = { x, y };
				Vec3 currentPos = gridOrigin + Vec3(x * tileSize, 0.0f, y * tileSize);

				float distance = glm::distance(waterPos, currentPos);
				if (distance <= 1.0f && grid.getType(Ivec2(x, y)) != "water" && grid.getType(Ivec2(x, y)) != "road")
				{
					nearbyTiles.push_back(currentTile);
				}
			}
		}
	}


	for (const auto& [plantType, count] : waterPlants) {

		if (count <= 0)
			continue;

		// Group count
		int groupCount = static_cast<int>(std::round(count * (1.0f - clumpiness) + 1.0f * clumpiness));
		groupCount = std::max(1, groupCount);

		// Group radius
		float groupRadius = 40.0f * clumpiness * 0.02f; // TWO MAGIC NUMBERS, clear them later

		// Group centers 
		std::vector<Vec3> groupCenters;

		if (waterTiles.empty()) {
			logDebug("No tiles near water - which is impossible.");
			continue;
		}

		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> tilePicker(0, static_cast<int>(nearbyTiles.size()) - 1);

		for (int i = 0; i < groupCount; ++i) {

			Ivec2 tile = nearbyTiles[tilePicker(rng)];

			float x = randf(tile.x * tileSize, (tile.x + 1) * tileSize);
			float z = randf(tile.y * tileSize, (tile.y + 1) * tileSize);

			Vec3 groupCenter = gridOrigin + Vec3(x, 0.0f, z);

			groupCenters.push_back(groupCenter);
		}

		// Group distributions --------
		std::vector<float> groupWeights(groupCount);
		float totalWeight = 0.0f;
		std::vector<int> plantsPerGroup(groupCount);

		// Weights 
		for (int i = 0; i < groupCount; ++i) {
			groupWeights[i] = randf(0.5f, 1.5f);  // MAGIC NUMBERS HERE TOO: we could use a parameter for weight variation
			totalWeight += groupWeights[i];
		}

		// Normalization of the weights
		for (int i = 0; i < groupCount; ++i) {
			groupWeights[i] /= totalWeight;
		}

		// Assign plant counts per group
		int assignedPlants = 0;

		for (int i = 0; i < groupCount; ++i) {
			plantsPerGroup[i] = static_cast<int>(std::round(groupWeights[i] * count));
			assignedPlants += plantsPerGroup[i];
		}

		// Fix rounding errors
		while (assignedPlants > count) {
			for (int i = 0; i < groupCount && assignedPlants > count; ++i) {
				if (plantsPerGroup[i] > 0) {
					plantsPerGroup[i] -= 1;
					assignedPlants -= 1;
				}
			}
		}

		while (assignedPlants < count) {
			for (int i = 0; i < groupCount && assignedPlants < count; ++i) {
				plantsPerGroup[i] += 1;
				assignedPlants += 1;
			}
		}

		// Generate the plants ---

		for (int groupIndex = 0; groupIndex < groupCount; ++groupIndex) {

			Vec3 groupCenter = groupCenters[groupIndex];
			float adjustedRadius = groupRadius * groupWeights[groupIndex];
			int plantCount = plantsPerGroup[groupIndex];

			// Collect valid tiles in range
			std::vector<Ivec2> validTilesInRange;

			for (const Ivec2& tile : nearbyTiles) {

				Vec3 tileCenter = gridOrigin + Vec3(
					tile.x * tileSize + tileSize * 0.5f,
					0.0f,
					tile.y * tileSize + tileSize * 0.5f
				);

				float distance = glm::distance(groupCenter, tileCenter);

				if (distance <= adjustedRadius + tileSize * 0.5f) {
					validTilesInRange.push_back(tile);
				}
			}

			if (validTilesInRange.empty())
				continue;

			std::uniform_int_distribution<int> tilePicker(0, static_cast<int>(validTilesInRange.size()) - 1);

			// Spawn plants randomly in valid tiles

			for (int p = 0; p < plantCount; ++p) {

				Ivec2 tile = validTilesInRange[tilePicker(rng)];

				float x = randf(tile.x * tileSize, (tile.x + 1) * tileSize);
				float z = randf(tile.y * tileSize, (tile.y + 1) * tileSize);

				Vec3 plantPosition = gridOrigin + Vec3(x, 0.0f, z);

				switch (plantType) {

				case EdiblePlantTypes::ReedA:
					createReed1(plantPosition);
					break;

				case EdiblePlantTypes::ReedB:
					createReed2(plantPosition);
					break;


				default:
					break;
				}

			}
		}

	}

}


void FloraManager::update(Entity& entity) {

	if (Input::isKeyPressed(Input::Key::A)) {
		
		generateInitialPlants(1);

	}

	if (Input::isKeyPressed(Input::Key::D)) {

		std::unordered_map<EdiblePlantTypes, int> waterPlants = {
			{ EdiblePlantTypes::ReedA, 6 },
			{ EdiblePlantTypes::ReedB, 4 }
		};

		generateInitialWaterPlants(1, waterPlants);

	}

}