#include "RandomMigration.hpp"

RandomMigration::RandomMigration() {}

void RandomMigration::start(Entity& entity) {
	// TEMP TEMP
	static std::random_device rd2;
	static std::mt19937 gen2(rd2());
	static std::uniform_int_distribution<int> dist2(0, 1600);
	// TEMP
	counterMax = 1600 + dist2(gen2);
	counter = counterMax;
}

void RandomMigration::update(Entity& entity) {

	if (entity.getComponent<DestinationBasedMovement>().getArrived() && counter > 0) {
		counter--;
	}

	if (counter <= 0) {
		entity.getComponent<DestinationBasedMovement>().setDestination(Vec3(randf(-20, +20),0, randf(-20, +20)));
		entity.getComponent<DestinationBasedMovement>().updatePath();
		counter = counterMax;
	}

}
