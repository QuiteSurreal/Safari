#include "HerbivoreHerdAI.hpp"

HerbivoreHerdAI::HerbivoreHerdAI() : Behaviour() {

	type = HerdTypes::Zebra;

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

}


// TEMP
void HerbivoreHerdAI::updateUniqueRoaming(Entity& ent) {
	ent.getComponent<HerbivoreAI>().setIdleCounter(0);
}


void HerbivoreHerdAI::calculateGeneralMood() {

	// EXPERIMENTAL !!!!!!!!

	float neighborhoodEffect = std::tanh(neighborEffect * 0.1f);
	float rawMood = overallSatiety * 0.5f + (1.0f - frightenedMeter) * 0.3f + neighborhoodEffect * 0.2f;
	generalMood = std::clamp(rawMood, 0.0f, 1.0f);

}


void HerbivoreHerdAI::assessAnimalSatiety() {

	if (herdSize > 0) {

		float averageFoodPercentage = Labels::getUnique(herdLabel + ":" + std::to_string(0)).getComponent<HerbivoreAI>().calculateFoodPercentage();
		float minimumFoodPercentage = averageFoodPercentage;

		float averageWaterPercentage = Labels::getUnique(herdLabel + ":" + std::to_string(0)).getComponent<HerbivoreAI>().calculateWaterPercentage();
		float minimumWaterPercentage = averageFoodPercentage;

		float fp;
		float wp;
		for (int i = 1; i < herdSize; i++) {
			Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
			fp = ent.getComponent<HerbivoreAI>().calculateFoodPercentage();
			wp = ent.getComponent<HerbivoreAI>().calculateWaterPercentage();
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


void HerbivoreHerdAI::assessThreat() {

	if (herdSize > 0) {

		float averageSpookLevel = 0;

		for (int i = 0; i < herdSize; i++) {
			Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
			averageSpookLevel += ent.getComponent<HerbivoreAI>().calculateSpookPercentage();
		}

		averageSpookLevel = averageSpookLevel / herdSize;
		frightenedMeter = averageSpookLevel;

	}
	else {

		frightenedMeter = 0;

	}

}


void HerbivoreHerdAI::assessNeighbors()
{

	float neighborModifier = 0;

	// Check every species the herd is in a mood-modifying relationship with.
	// Calculate the neighborhood modifier based on the presence of these animals.
	int counter;
	for (const auto& [speciesLabel, modifier] : speciesRelationships)
	{

		counter = 0;
		const auto& entities = Labels::getAll(speciesLabel); // TODO !!!!! Need to add species speficic labels to the herds

		if (entities.empty()) {
			continue;
		}

		for (Entity* ent : entities)
		{
			counter += ent->getComponent<HerbivoreHerdAI>().getIntersectingAnimals(entity->getComponent<Transform>().getPosition(), roamRadius);
		}

		neighborModifier += (modifier * counter);

	}

	neighborEffect = neighborModifier;

}


// WE NEED TO CALL THIS WHEN REMOVING AN ANIMAL
// waiting for final implementation for the latter
void HerbivoreHerdAI::calculateMigratePositions() {

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

const std::tuple<int, int, std::string, std::string> HerbivoreHerdAI::addAnimal() {

	herdMigratePositions.push_back(Vec3(0.0f, 0.0f, 0.0f));
	herdSize++;
	std::string newAnimalLabel = herdLabel + ":" + std::to_string(herdSize-1);
	return std::tuple<int, int, std::string, std::string>(globalHerdId, herdSize-1, herdLabel, newAnimalLabel);

}


void HerbivoreHerdAI::removeAnimalFromHerd(int localId) {

	std::string animalLabel = herdLabel + ":" + std::to_string(localId);
	Entity& ent = Labels::getUnique(animalLabel);
	ent.getComponent<HerbivoreAI>().setHerdId(-1);
	ent.getComponent<Labels>().removeLabel(animalLabel);
	for (int i = (localId + 1); i < herdSize; ++i)
	{
		Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
		ent.getComponent<HerbivoreAI>().setLocalId(i - 1);
		ent.getComponent<Labels>().removeLabel(herdLabel + ":" + std::to_string(i));
		ent.getComponent<Labels>().addLabel(herdLabel + ":" + std::to_string(i - 1));
	}

	--herdSize;

}


std::vector<std::string> HerbivoreHerdAI::getAnimalIds() {

	std::vector<std::string> ids;
	for (int i = 0; i < herdSize; ++i) {
		ids.push_back(herdLabel + ":" + std::to_string(i));
	}

	return ids;

}


void HerbivoreHerdAI::addSpeciesRelationship(const std::string& speciesLabel, float modifier) {
	speciesRelationships[speciesLabel] = modifier;
}


void HerbivoreHerdAI::removeSpeciesRelationship(const std::string& speciesLabel) {
	auto it = speciesRelationships.find(speciesLabel);
	if (it != speciesRelationships.end()) {
		speciesRelationships.erase(it);
	}
}


Entity* HerbivoreHerdAI::pickValidFoodSource() {

	if (!atSite) {
		logDebug("It's not quite the right time to get hungry, my brother.");
		return nullptr;
	}

	return getEntity().getComponent<HerbivoreSiteMemory>().pickFoodSourceAtSite(getEntity().getComponent<Transform>().getPosition());
	
}


std::optional<Vec3> HerbivoreHerdAI::pickValidWaterSource() {

	if (!atSite) {
		logDebug("It's not quite the right time to get thirsty, my brother.");
		return std::nullopt;
	}

	return getEntity().getComponent<HerbivoreSiteMemory>().pickWaterSourceAtSite(getEntity().getComponent<Transform>().getPosition());

}


void HerbivoreHerdAI::start(Entity& entity) {
	reproductionCounter = maxReproductionCounter;
}


void HerbivoreHerdAI::update(Entity& entity) {

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
			{
				//char buf[64];
				//std::snprintf(buf, sizeof(buf), "Satiety: %.2f  |  Hydration: %.2f", overallSatiety, overallHydration);
				//logDebug(buf);
			}
			// DEBUG
			
			// |-------------------------------------------------------------|
			// |###| General state calculations |############################|
			// |-------------------------------------------------------------|

			assessAnimalSatiety();
			assessNeighbors();
			calculateGeneralMood();



			// |-------------------------------------------------------------|
			// |###| Rest & Nourish controller |#############################|
			// |-------------------------------------------------------------|
			
			// If the herd is not at a site, checks if it should be heading towards one.
			if (!atSite) {

				if ((overallSatiety < satietyComfortLimit || minimumSatiety < satietyCriticalLimit || overallHydration < hydrationComfortLimit || minimumHydration < hydrationCriticalLimit) && (state != State::HeadingToPreferred || /*entity.getComponent<HerbivoreSiteMemory>().isThereAValidSite()*/ false)) {
					enterHeadingToPreferredState();
				}

			}
			// Herd is at site.
			else {

				// No more food at the spot.
				if (entity.getComponent<HerbivoreSiteMemory>().foodDepleted(entity.getComponent<Transform>().getPosition())) {

					// Herd is fine: wander to random position.
					if (overallSatiety >= satietyComfortLimit && minimumSatiety >= satietyCriticalLimit && overallHydration >= hydrationComfortLimit && minimumHydration >= hydrationCriticalLimit) {
						atSite = false;
						enterWanderingState();
					}
					// Herd is still in need of resources: find another site.
					else {
						atSite = false;
						enterHeadingToPreferredState();
					}
					
				}

				// Herd if fully satiated.
				if (overallSatiety > 99.0f && overallHydration > 99.0f) {
					atSite = false;
					enterWanderingState();
				}

			}

		}

		// >>--------<<


		// <------ << Every frame >> ------>>

		// If the herd arrives to the destination, enter resting state.
		// In case the herd was en route to a site, denote arrival.
		if (entity.getComponent<DestinationBasedMovement>().getArrived()) {
			if (state == State::HeadingToPreferred) {
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
		if (state == State::Wandering || state == State::HeadingToPreferred) {
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


int HerbivoreHerdAI::getIntersectingAnimals(Vec3 otherHerdCenter, float otherHerdTerritoryRadius) {

	int count = 0;

	for (int i = 0; i < herdSize; i++) {
		Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
		Vec3 pos = ent.getComponent<Transform>().getPosition();
		if (glm::distance(pos, otherHerdCenter) < otherHerdTerritoryRadius) {
			count++;
		}
	}

	return count;

}




void HerbivoreHerdAI::updateAnimalRoaming() {

	// Could be optimized by throttling.

	for (int i = 0; i < herdSize; i++) {
		Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
		if (ent.getComponent<HerbivoreAI>().getIdleCounter() <= 0 && ent.getComponent<HerbivoreAI>().getState() == "Neutral") {
			ent.getComponent<DestinationBasedMovement>().setDestination(getRandomAnimalRoamingPosition());
			ent.getComponent<DestinationBasedMovement>().updatePath();
			ent.getComponent<HerbivoreAI>().setIdleCounter(ent.getComponent<HerbivoreAI>().getIdleMax());
		}
	}

}


Vec3 HerbivoreHerdAI::getRandomAnimalRoamingPosition() {

	// Get's a random position inside the roaming perimeter, avoiding clustering.

	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	float theta = dist(gen) * 2.0f * PI;  
	float r = roamRadius * std::sqrt(dist(gen));
	return Vec3(r * std::cos(theta), 0.0f, r * std::sin(theta)) + entity->getComponent<Transform>().getPosition();

}

void HerbivoreHerdAI::updateAnimalMigration(Entity& entity) {

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
		if (ent.getComponent<HerbivoreAI>().getState() == "Neutral") {
			ent.getComponent<DestinationBasedMovement>().setDestination(herdMigratePositions[i]);
			ent.getComponent<DestinationBasedMovement>().updatePath();
		}
		
	}

}




void HerbivoreHerdAI::updateReproduction() {

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


void HerbivoreHerdAI::initiateMating() {

	if (!reproductionInProgress) {

		//logDebug("sex is about to take place");

		std::vector<Entity*> males;
		std::vector<Entity*> females;

		// Sorting animals into vectors by sex.
		for (int i = 0; i < herdSize; ++i) {

			Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
			Sexes sex = ent.getComponent<HerbivoreAI>().getSex();

			if (!ent.getComponent<HerbivoreAI>().isAlive()) continue;

			if (sex == Sexes::Male && ent.getComponent<HerbivoreAI>().getAge() >= ent.getComponent<HerbivoreAI>().getAdultAgeFloor()) {
				males.push_back(&ent);
			}
			else if (sex == Sexes::Female && ent.getComponent<HerbivoreAI>().getAge() >= ent.getComponent<HerbivoreAI>().getAdultAgeFloor()) {
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

			male->getComponent<HerbivoreAI>().breed(female);
			reproductionInProgress = true;

		}
	}

}


void HerbivoreHerdAI::signalAtSiteToAnimals(bool atSite) {

	for (int i = 0; i < herdSize; i++) {

		Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
		ent.getComponent<HerbivoreAI>().setAtSite(atSite);

	}

}


void HerbivoreHerdAI::enterWanderingState() {

	signalAtSiteToAnimals(false);

	calculateMigratePositions();
	entity->getComponent<DestinationBasedMovement>().setDestination(Vec3(randf(-20, +20), 0, randf(-20, +20)));
	entity->getComponent<DestinationBasedMovement>().updatePath();

	state = State::Wandering;

}


void HerbivoreHerdAI::enterHeadingToPreferredState() {

	signalAtSiteToAnimals(false);

	calculateMigratePositions();
	entity->getComponent<DestinationBasedMovement>().setDestination(entity->getComponent<HerbivoreSiteMemory>().pickSite());
	entity->getComponent<DestinationBasedMovement>().updatePath();

	state = State::HeadingToPreferred;

}


void HerbivoreHerdAI::enterRestingState() {

	restCounter = maxRestCounter;

	if (atSite) {
		signalAtSiteToAnimals(true);
	}

	state = State::Resting;

}

