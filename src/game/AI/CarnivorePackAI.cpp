#include "CarnivorePackAI.hpp"

CarnivorePackAI::CarnivorePackAI() : Behaviour() {

	type = HerdTypes::Lion;

	accumulator = 0.0f;
	timeStep = 1.0f / 60.0f;
	everyFrameTimer = 0.0f;
	every6FrameTimer = 0.0f;
	every30FrameTimer = 0.0f;
	every60FrameTimer = 0.0f;

	throttleCounter = 0;

	globalHerdId = 0;
	herdLabel = "unassigned";

	maxHerdSize = 0;
	herdSize = 0;

	fovRadius = 0.0f;
	roamRadius = 0.0f;
	migrateRadius = 0.0f;

	generalMood = 0.0f;

	overallSatiety = 0.0f;
	minimumSatiety = 0.0f;

	overallHydration = 0.0f;
	minimumHydration = 0.0f;

	satietyComfortLimit = 0.0f;
	satietyCriticalLimit = 0.0f;

	hydrationComfortLimit = 0.0f;
	hydrationCriticalLimit = 0.0f;

	atSite = false;

	neighborEffect = 0.0f;

	state = State::Wandering;

	maxRestCounter = 2000;
	restCounter = 0;
	//maxRestfulness = 0;
	//restfulness = 0;
	maxRestCounter = 2000;
	restCounter = 0;

	frightened = false;
	frightenedThreshold = 0;
	frightenedMeter = 0;

	reproductionInProgress = false;


	commonPrey = nullptr;

}


// TEMP
void CarnivorePackAI::updateUniqueRoaming(Entity& ent) {
	ent.getComponent<CarnivoreAI>().setIdleCounter(0);
}


void CarnivorePackAI::calculateGeneralMood() {

	// EXPERIMENTAL !!!!!!!!

	float neighborhoodEffect = std::tanh(neighborEffect * 0.1f);
	float rawMood = overallSatiety * 0.5f + neighborhoodEffect * 0.2f;
	generalMood = std::clamp(rawMood, 0.0f, 1.0f);

}


void CarnivorePackAI::assessAnimalSatiety() {

	if (herdSize > 0) {

		float averageFoodPercentage = Labels::getUnique(herdLabel + ":" + std::to_string(0)).getComponent<CarnivoreAI>().calculateFoodPercentage();
		float minimumFoodPercentage = averageFoodPercentage;

		float averageWaterPercentage = Labels::getUnique(herdLabel + ":" + std::to_string(0)).getComponent<CarnivoreAI>().calculateWaterPercentage();
		float minimumWaterPercentage = averageFoodPercentage;

		float fp;
		float wp;
		for (int i = 1; i < herdSize; i++) {
			Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
			fp = ent.getComponent<CarnivoreAI>().calculateFoodPercentage();
			wp = ent.getComponent<CarnivoreAI>().calculateWaterPercentage();
			averageFoodPercentage += fp;
			averageWaterPercentage += wp;
			if (fp < minimumFoodPercentage) {
				minimumFoodPercentage = fp;
			}
			if (wp < minimumWaterPercentage) {
				minimumWaterPercentage = wp;
			}
		}

		averageFoodPercentage = averageFoodPercentage / herdSize;
		averageWaterPercentage = averageWaterPercentage / herdSize;

		overallSatiety = averageFoodPercentage;
		minimumSatiety = minimumFoodPercentage;

		overallHydration = averageWaterPercentage;
		minimumHydration = minimumWaterPercentage;

	}
	else {

		overallSatiety = 0;
		minimumSatiety = 0;

	}

}


// WE NEED TO CALL THIS WHEN REMOVING AN ANIMAL
// waiting for final implementation for the latter
void CarnivorePackAI::calculateMigratePositions() {

	int n = herdSize;
	herdMigratePositions.clear();

	const double goldenAngle = PI * (3.0 - std::sqrt(5.0));

	std::vector<int> v(n);
	std::iota(v.begin(), v.end(), 0);
	std::shuffle(v.begin(), v.end(), std::mt19937{ std::random_device{}() });

	for (int i : v) {

		double r = migrateRadius * std::sqrt((double)i / (n - 1));
		double theta = i * goldenAngle;

		herdMigratePositions.push_back( Vec3(r * std::cos(theta), 0.0f, r * std::sin(theta)) + entity->getComponent<Transform>().getPosition() );

	}

}

const std::tuple<int, int, std::string, std::string> CarnivorePackAI::addAnimal() {

	herdMigratePositions.push_back(Vec3(0.0f, 0.0f, 0.0f));
	herdSize++;
	std::string newAnimalLabel = herdLabel + ":" + std::to_string(herdSize-1);
	return std::tuple<int, int, std::string, std::string>(globalHerdId, herdSize-1, herdLabel, newAnimalLabel);

}


void CarnivorePackAI::removeAnimalFromHerd(int localId) {

	std::string animalLabel = herdLabel + ":" + std::to_string(localId);
	Entity& ent = Labels::getUnique(animalLabel);
	ent.getComponent<CarnivoreAI>().setHerdId(-1);
	ent.getComponent<Labels>().removeLabel(animalLabel);
	for (int i = (localId + 1); i < herdSize; ++i)
	{
		Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
		ent.getComponent<CarnivoreAI>().setLocalId(i - 1);
		ent.getComponent<Labels>().removeLabel(herdLabel + ":" + std::to_string(i));
		ent.getComponent<Labels>().addLabel(herdLabel + ":" + std::to_string(i - 1));
	}

	--herdSize;

	if (herdSize == 0)
	{
		entity->destroy();
	}

}


std::vector<std::string> CarnivorePackAI::getAnimalIds() {

	std::vector<std::string> ids;
	for (int i = 0; i < herdSize; ++i) {
		ids.push_back(herdLabel + ":" + std::to_string(i));
	}

	return ids;

}




Entity* CarnivorePackAI::acquirePrey() {

	if (!atSite) {
		logDebug("It's not quite the right time to get hungry, my brother.");
		return nullptr;
	}

	return commonPrey;
	
	
}


std::optional<Vec3> CarnivorePackAI::pickValidWaterSource() {

	if (!atSite) {
		logDebug("It's not quite the right time to get thirsty, my brother.");
		return std::nullopt;
	}

	return pickWaterSource();

}


void CarnivorePackAI::start(Entity& entity) {
	reproductionCounter = maxReproductionCounter;
}


void CarnivorePackAI::update(Entity& entity) {

	// DEBUG
	if (Input::isKeyPressed(Input::Key::W)) {
		initiateMating();
	}
	// DEBUG

	accumulator += Input::getDeltaTime();

	while (accumulator >= timeStep) {

		// <------ << Timers >> ------>

		accumulator -= timeStep;
		everyFrameTimer += timeStep;
		every6FrameTimer += timeStep;
		every30FrameTimer += timeStep;
		every60FrameTimer += timeStep;

		// >>--------<<


		// <------ << Every 6 frames >> ------>
		if (every6FrameTimer >= 0.1f) {
			
			// DEBUG
			/*{
				char buf[64];
				std::snprintf(buf, sizeof(buf), "Satiety: %.2f  |  Hydration: %.2f", overallSatiety, overallHydration);
				logDebug(buf);
			}*/
			// DEBUG
			
			// |-------------------------------------------------------------|
			// |###| General state calculations |############################|
			// |-------------------------------------------------------------|

			assessAnimalSatiety();
			calculateGeneralMood();



			// |-------------------------------------------------------------|
			// |###| Rest & Nourish controller |#############################|
			// |-------------------------------------------------------------|
			
			// If the herd is not at a site, checks if it should be heading towards one.
			if (!atSite) {

				if ((overallSatiety < satietyComfortLimit || overallHydration < hydrationComfortLimit) && (state != State::Hunting || /*entity.getComponent<HerbivoreSiteMemory>().isThereAValidSite()*/ false)) {
					enterHuntingState();
				}

			}
			// Herd is at site.
			else {

				// Herd if fully satiated.
				if (overallSatiety > 0.8f && overallHydration > 0.8f) {
					atSite = false;
					signalNearPreyToAnimals(false);
					// KILL the animal
					commonPrey = nullptr;
					enterWanderingState();
				}
				else {
					logDebug("FIRE WILL RAIN PREPARE TO DIE.");
				}

			}

		}

		// >>--------<<


		// <------ << Every frame >> ------>>

		// If the herd arrives to the destination, enter resting state.
		// In case the herd was en route to a site, denote arrival.
		if (entity.getComponent<DestinationBasedMovement>().getArrived()) {
			if (state == State::Hunting) {
				atSite = true;
			}
			if (state != State::Resting)
				enterRestingState();
		}

		// When resting (at a site or at a random position).
		if (state == State::Resting) {

			// Update the random roaming: check cooldown, pick destination for the animal.
			updateAnimalRoaming();

			// Update the reproduction behaviour. If the herd limit is not reached, freaky things can happen.
			if (!reproductionInProgress && herdSize < maxHerdSize) {
				updateReproduction();
			}

			// If the herd is not stationing at a food site, the rest counter is updated. When the herd is well-rested,
			// they wander to a new destionation.
			if (!atSite && --restCounter <= 0) {
				enterWanderingState();
			}

		}

		// When moving: update the herd migration formation.
		if (state == State::Wandering || state == State::Hunting) {
			updateAnimalMigration(entity);
		}

		// >>--------<<


		// <------ << Timer handling >> ------>

		if (every6FrameTimer >= 0.1f)
			every6FrameTimer -= 0.1f;

		if (every30FrameTimer >= 0.5f)
			every30FrameTimer -= 0.5f;

		if (every60FrameTimer >= 1.0f)
			every60FrameTimer -= 1.0f;

		// >>--------<<

	}

}


void CarnivorePackAI::updateAnimalRoaming() {

	// Could be optimized by throttling.

	for (int i = 0; i < herdSize; i++) {
		Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
		if (ent.getComponent<CarnivoreAI>().getIdleCounter() <= 0 && ent.getComponent<CarnivoreAI>().getState() == "Neutral") {
			ent.getComponent<DestinationBasedMovement>().setDestination(getRandomAnimalRoamingPosition());
			ent.getComponent<DestinationBasedMovement>().updatePath();
			ent.getComponent<CarnivoreAI>().setIdleCounter(ent.getComponent<CarnivoreAI>().getIdleMax());
		}
	}

}


Vec3 CarnivorePackAI::getRandomAnimalRoamingPosition() {

	// Get's a random position inside the roaming perimeter, avoiding clustering.

	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	float theta = dist(gen) * 2.0f * PI;  
	float r = roamRadius * std::sqrt(dist(gen));
	return Vec3(r * std::cos(theta), 0.0f, r * std::sin(theta)) + entity->getComponent<Transform>().getPosition();

}

void CarnivorePackAI::updateAnimalMigration(Entity& entity) {

	Vec3 herdMovement;
	if (entity.getComponent<DestinationBasedMovement>().getDestination() != entity.getComponent<Transform>().getPosition()) {
		herdMovement = normalize(entity.getComponent<DestinationBasedMovement>().getDestination() - entity.getComponent<Transform>().getPosition()) * entity.getComponent<DestinationBasedMovement>().getSpeed();
	}
	else {
		herdMovement = Vec3(0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < herdSize; i++) {

		herdMigratePositions[i] += herdMovement;

		// Can be optimized
		Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
		if (ent.getComponent<CarnivoreAI>().getState() == "Neutral") {
			ent.getComponent<DestinationBasedMovement>().setDestination(herdMigratePositions[i]);
			ent.getComponent<DestinationBasedMovement>().updatePath();
		}
		
	}

}




void CarnivorePackAI::updateReproduction() {

	// The variable for calculating a modifier for the reproduction counter.
	float adjustedMood = std::clamp(generalMood - 0.6f, 0.0f, 1.0f);

	// Animals are obstinate when it comes to mating. They only get horny when their general mood is
	// above a certain threshold, meaning both their physiological and psychological needs are met, and they feel safe.
	// That's one major difference between them and me.

	// [TODO]: threat

	reproductionCounter -= std::floor(adjustedMood * 100);

	if (reproductionCounter <= 0) {
		initiateMating();
		reproductionCounter = maxReproductionCounter;
	}

}


void CarnivorePackAI::initiateMating() {

	if (!reproductionInProgress) {

		//logDebug("sex is about to take place");

		std::vector<Entity*> males;
		std::vector<Entity*> females;

		// Sorting animals into vectors by sex.
		for (int i = 0; i < herdSize; ++i) {

			Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
			Sexes sex = ent.getComponent<CarnivoreAI>().getSex();

			if (!ent.getComponent<CarnivoreAI>().isAlive()) continue;

			if (sex == Sexes::Male && ent.getComponent<CarnivoreAI>().getAge() >= ent.getComponent<CarnivoreAI>().getAdultAgeFloor()) {
				males.push_back(&ent);
			}
			else if (sex == Sexes::Female && ent.getComponent<CarnivoreAI>().getAge() >= ent.getComponent<CarnivoreAI>().getAdultAgeFloor()) {
				females.push_back(&ent);
			}

		}

		// Ensure both pools have at least one animal.
		// Otherwise, we have a big problem.
		// [TODO] How to solve this though?
		if (!males.empty() && !females.empty()) {

			static std::mt19937 gen(std::random_device{}());

			std::uniform_int_distribution<int> maleDist(0, males.size() - 1);
			std::uniform_int_distribution<int> femaleDist(0, females.size() - 1);

			Entity* male = males[maleDist(gen)];
			Entity* female = females[femaleDist(gen)];

			male->getComponent<CarnivoreAI>().breed(female);
			reproductionInProgress = true;

		}
	}

}


void CarnivorePackAI::signalNearPreyToAnimals(bool atSite) {

	for (int i = 0; i < herdSize; i++) {

		Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
		ent.getComponent<CarnivoreAI>().setAtSite(atSite);

	}

}


void CarnivorePackAI::enterWanderingState() {

	signalNearPreyToAnimals(false);

	calculateMigratePositions();
	entity->getComponent<DestinationBasedMovement>().setDestination(Vec3(randf(-20, +20), 0, randf(-20, +20)));
	entity->getComponent<DestinationBasedMovement>().updatePath();

	state = State::Wandering;

}


void CarnivorePackAI::enterHuntingState() {

	signalNearPreyToAnimals(false);

	calculateMigratePositions();
	


	std::vector<HerdTypes> herbivoreTypes = { HerdTypes::Zebra, HerdTypes::Wildebeest };

	static std::mt19937 rng{ std::random_device{}() };

	std::uniform_int_distribution<int> typePick(0, static_cast<int>(herbivoreTypes.size()) - 1);
	HerdTypes chosenType = herbivoreTypes[typePick(rng)];

	int herdCount = Labels::getUnique("eco").getComponent<HerdManager>().getNextHerdId(chosenType);

	std::uniform_int_distribution<int> herdPick(0, herdCount - 1);
	int herdId = herdPick(rng);

	std::string herdLabel = (chosenType == HerdTypes::Zebra ? "zebraHerd:" : "wildebeestHerd:") + std::to_string(herdId);

	Entity& herdEntity = Labels::getUnique(herdLabel);
	HerbivoreHerdAI& ai = herdEntity.getComponent<HerbivoreHerdAI>();

	int herdSize = ai.getHerdSize();

	std::uniform_int_distribution<int> animalPick(0, herdSize - 1);
	std::string animalLabel = herdLabel + ":" + std::to_string(animalPick(rng));

	commonPrey = &(Labels::getUnique(animalLabel));
	
	entity->getComponent<DestinationBasedMovement>().setDestination(commonPrey->getComponent<Transform>().getPosition());
	entity->getComponent<DestinationBasedMovement>().updatePath();
	state = State::Hunting;


	/*if (commonPrey->getComponent<Targetable>().canTarget()) {
		entity->getComponent<Targetable>().target();
		entity->getComponent<DestinationBasedMovement>().setDestination(commonPrey->getComponent<Transform>().getPosition());
		entity->getComponent<DestinationBasedMovement>().updatePath();
		state = State::Hunting;
	}
	else {
		commonPrey = nullptr;
		enterWanderingState();
	}*/

}


void CarnivorePackAI::enterRestingState() {

	restCounter = maxRestCounter;

	if (atSite) {
		signalNearPreyToAnimals(true);
	}

	state = State::Resting;

}




std::optional<Vec3> CarnivorePackAI::pickWaterSource()
{
	Vec3 position = getEntity().getComponent<Transform>().getPosition();

	Entity& baseGrid = Labels::getUnique("global");
	Grid& grid = baseGrid.getComponent<Grid>();
	const auto& waterTiles = grid.getAllOfType("water");
	if (waterTiles.empty())
		return std::nullopt;
	float tileSize = grid.getTileSize();
	Vec3 origin = grid.getPosition();

	std::vector<Ivec2> valid;
	for (const Ivec2& tile : waterTiles)
	{

		float minX = origin.x + tile.x * tileSize;
		float maxX = origin.x + (tile.x + 1) * tileSize;
		float minZ = origin.z + tile.y * tileSize;
		float maxZ = origin.z + (tile.y + 1) * tileSize;

		float dx = std::max({ minX - position.x, 0.f, position.x - maxX });
		float dz = std::max({ minZ - position.z, 0.f, position.z - maxZ });

		if (std::sqrt(dx * dx + dz * dz) <= 20.0f)
			valid.push_back(tile);

	}

	if (valid.empty())
		return std::nullopt;

	static std::mt19937 rng{ std::random_device{}() };
	std::uniform_int_distribution<int> pick(0, static_cast<int>(valid.size()) - 1);
	Ivec2 chosenTile = valid[pick(rng)];

	float x = randf(chosenTile.x * tileSize, (chosenTile.x + 1) * tileSize);
	float z = randf(chosenTile.y * tileSize, (chosenTile.y + 1) * tileSize);

	return origin + Vec3(x, 0.f, z);

}
