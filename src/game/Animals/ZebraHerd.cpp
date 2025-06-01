#include "ZebraHerd.hpp"

ZebraHerd::ZebraHerd() : Behaviour() {

	globalHerdId = 0;
	herdLabel = "unassigned";
	fovRadius = 0;
	roamRadius = 0;
	migrateRadius = 0;
	herdSize = 0;
	state = State::Wandering;
}

// TEMP
void ZebraHerd::updateUniqueRoaming(Entity& ent) {
	ent.getComponent<Zebra>().setIdleCounter(0);
}

void ZebraHerd::updateState() {

	// TODO based on innter state

}

bool ZebraHerd::areAnimalsSatisfied() {
	return false; // TODO
}

void ZebraHerd::assessThreat() {} // TODO 

void ZebraHerd::assessNeighbors() {} // TODO

void ZebraHerd::calculateMigratePositions() {

	int n = herdSize;
	herdMigratePositions.clear();

	const double goldenAngle = PI * (3.0 - std::sqrt(5.0));

	//std::vector<int> v(n);
	std::vector<int> v(n);
	std::iota(v.begin(), v.end(), 0);
	std::shuffle(v.begin(), v.end(), std::mt19937{ std::random_device{}() });

	for (int i : v) {
		double r = migrateRadius * std::sqrt((double)i / (n - 1));
		double theta = i * goldenAngle;

		herdMigratePositions.push_back( Vec3(r * std::cos(theta), 0.0f, r * std::sin(theta)) + entity->getComponent<Transform>().getPosition() );
	}

}

void ZebraHerd::assignUniqueHerdId(int id) {

	globalHerdId = id;
	herdLabel = "zebraHerd:" + std::to_string(globalHerdId);

}

const std::tuple<int, int, std::string> ZebraHerd::addAnimal() {

	herdMigratePositions.push_back(Vec3(0.0f, 0.0f, 0.0f));
	herdSize++;
	std::string newAnimalLabel = herdLabel + ":" + std::to_string(herdSize-1);
	return std::tuple<int, int, std::string>(globalHerdId, herdSize-1, newAnimalLabel);

}

void ZebraHerd::removeAnimalFromHerd(int localId) {
	std::string animalLabel = herdLabel + ":" + std::to_string(localId);
	Entity& ent = Labels::getUnique(animalLabel);
	ent.getComponent<Labels>().removeLabel(animalLabel);
	for (int i = (localId + 1); i < herdSize; ++i)
	{
		Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
		ent.getComponent<Zebra>().setLocalId(i - 1);
		ent.getComponent<Labels>().removeLabel(herdLabel + ":" + std::to_string(i));
		ent.getComponent<Labels>().addLabel(herdLabel + ":" + std::to_string(i - 1));
	}

	--herdSize;

}

std::vector<std::string> ZebraHerd::getAnimalIds() {

	std::vector<std::string> ids;
	for (int i = 0; i < herdSize; ++i) {
		ids.push_back("zebraHerd:" + std::to_string(globalHerdId) + ":" + std::to_string(i));
	}

	return ids;

}

void ZebraHerd::start(Entity& entity) { }

void ZebraHerd::update(Entity& entity) {
	// TEMPORARY <- until state machine is implemented
	if (entity.getComponent<DestinationBasedMovement>().getArrived()) {
		state = State::Resting;
	}
	else {
		if (state != State::Wandering) {
			calculateMigratePositions();
		}
		state = State::Wandering;
	}
	// TEMP

	if (state == State::Resting) {
		updateAnimalRoaming();
	}

	if (state == State::Wandering) {
		updateAnimalMigration(entity);
	}

}

void ZebraHerd::updateAnimalRoaming() {

	for (int i = 0; i < herdSize; i++) {
		Entity& ent = Labels::getUnique(herdLabel + ":" + std::to_string(i));
		if (ent.getComponent<Zebra>().getIdleCounter() <= 0 && ent.getComponent<Zebra>().getState() == "Neutral") {
			ent.getComponent<DestinationBasedMovement>().setDestination(getRandomAnimalRoamingPosition());
			ent.getComponent<DestinationBasedMovement>().updatePath();
			ent.getComponent<Zebra>().setIdleCounter(ent.getComponent<Zebra>().getIdleMax());
		}
	}

}



Vec3 ZebraHerd::getRandomAnimalRoamingPosition() {

	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

	float theta = dist(gen) * 2.0f * PI;  
	float r = roamRadius * std::sqrt(dist(gen));

	return Vec3(r * std::cos(theta), 0.0f, r * std::sin(theta)) + entity->getComponent<Transform>().getPosition();

}

void ZebraHerd::updateAnimalMigration(Entity& entity) {

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
		if (ent.getComponent<Zebra>().getState() == "Neutral") {
			ent.getComponent<DestinationBasedMovement>().setDestination(herdMigratePositions[i]);
			ent.getComponent<DestinationBasedMovement>().updatePath();
		}
		
	}

}

