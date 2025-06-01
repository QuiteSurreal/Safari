#include "HunterHut.hpp"



HunterHut::HunterHut(Vec3 position)
{
	pos = position;
    hutSize = 0;
    globalHutId = 0;
    hutLabel = "";
    deployHunter = false;
}

void HunterHut::start(Entity& entity)
{

}

void HunterHut::update(Entity& entity)
{
	Camera& camera = Labels::getUnique("global").getComponent<Camera>();
	Transform& trans = entity.getComponent<Transform>();
	rotate(camera, trans);


    //implement the creation in entity selector, the adding at the factory
    if (deployHunter && hutSize < 3)
    {

    }
}

void HunterHut::setUniqueId(int id)
{
	hutLabel = "hunterHut:" + std::to_string(id);
	entity->addComponent<Labels>().addLabel(hutLabel);
}

//a hut has a capacity of 3
const std::tuple<int, int, std::string> HunterHut::addHunter()
{
	std::string newHunterId = hutLabel + ":" + std::to_string(hutSize);
	return std::tuple<int, int, std::string>(globalHutId, hutSize, newHunterId);
    hutSize++;
}

void HunterHut::removeHunterFromHut(int localId) {

	std::string hunterLabel = hutLabel + ":" + std::to_string(localId);
	Entity& ent = Labels::getUnique(hunterLabel);
	ent.getComponent<Hunter>().setHutId(1);
	ent.getComponent<Labels>().removeLabel(hunterLabel);
	for (int i = (localId + 1); i < hutSize; ++i)
	{
		Entity& ent = Labels::getUnique(hutLabel + ":" + std::to_string(i));
		ent.getComponent<Hunter>().setLocalId(i - 1);
		ent.getComponent<Labels>().removeLabel(hutLabel + ":" + std::to_string(i));
		ent.getComponent<Labels>().addLabel(hutLabel + ":" + std::to_string(i - 1));
	}

	--hutSize;

}

void HunterHut::rotate(Camera& camera, Transform& trans)
{
	const Vec3& camPos = camera.getPos();
	const Vec3& cowPos = trans.getPosition();
	if (glm::distance(camPos, cowPos) > 1e-3f) {
		float angle = std::atan2(camPos.x - cowPos.x, camPos.z - cowPos.z);
		if (getEntity().hasComponent<DestinationBasedMovement>()) { // TODO: sometimes wrong
			const auto& move = getEntity().getComponent<DestinationBasedMovement>();
			const Vec3& dst = move.getDestination();
			const Vec3 vecDst = glm::normalize(dst - trans.getPosition());
			const Vec3 vecCam = glm::normalize(Vec3{ -camera.getForward().z, 0.0f, camera.getForward().x }); // TODO: right
			if (glm::dot(vecDst, vecCam) > 0) {
				angle += PI_F;
			}
		}
		trans.setRotation({ 0.0f, angle, 0.0f });
	}
}