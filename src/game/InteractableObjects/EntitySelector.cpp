#include "EntitySelector.hpp"

//this will be the selected entity, will implement later
Entity* EntitySelector::getSelected() const
{
	return selected;
}

const std::vector<Vec3> EntitySelector::getClicks(Input::MouseButton button) const
{
	std::vector<Vec3> result;

	for (const auto& click : clicks) {
		if (std::get<1>(click) == button) {
			result.push_back(std::get<0>(click));
		}
	}

	return result;
}


void EntitySelector::enable()
{
	enabled = true;
	selected = nullptr;
	clicks.clear();
}

void EntitySelector::disable()
{
	enabled = false;
	selected = nullptr;
	clicks.clear();
}

void EntitySelector::update(Entity&)
{
	if (enabled) {
		GameManager* gm = &Labels::getUnique("global").getComponent<GameManager>();

		const Camera& camera = Labels::getUnique("global").getComponent<Camera>();

		const Ray ray = camera.getRay(Input::getMousePosition());
		//const std::optional<Vec3> optPos = ray.castToYPlane();

		clicks.clear();
		for (auto [button, clickPos] : Input::getClickList()) {

			const Ray ray = camera.getRay(clickPos);
			const std::optional<Vec3> optPos = ray.castToYPlane();
			if (optPos) {
				Vec3 pos = *optPos;
				std::tuple<Vec3, Input::MouseButton> tuple = std::tuple(pos, button);
				clicks.push_back(tuple);
			}
		}

		

		//temp, might be changed later, maybe not, having everything here seems optimal to me, but we can check for clicks in every other behaviour too,
		//based on the clicks that come in here
		if (hunterDirectMode)
		{
			target = Selector::getSelected();
			if (target && target != selected)
			{
				if (target->getComponent<Labels>().hasLabel("animal") && target->getComponent<Targetable>().canTarget())
				{
					selected->getComponent<Hunter>().enterHunting(target);
					target = nullptr;
					selected = nullptr;
					Selector::removeSelected();
					hunterDirectMode = false;
				}
			}
			else
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					selected->getComponent<Hunter>().enterTravelling(pos);
					selected = nullptr;
					hunterDirectMode = false;
				}
			}
		}
		else if (uib.mode)
		{
			if (*uib.mode == "structure2")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					if (gm->buyItem(GameManager::Items::Camera))
					{
						Factory::createSecurityCamera(pos);
					}
				}
			}
			if (*uib.mode == "structure3")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					if (gm->buyItem(GameManager::Items::DroneStation))
					{
						Factory::createDroneStation(pos);
					}
				}
			}
			else if (*uib.mode == "structure4")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					if (gm->buyItem(GameManager::Items::Balloon))
					{
						Factory::createAirship(pos);
					}
				}
			}
			else if (*uib.mode == "structure5")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					if (gm->buyItem(GameManager::Items::HunterHut))
					{
						Factory::createHunterHut(pos);
					}
				}
			}
			else if (*uib.mode == "plant1")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					if (gm->buyItem(GameManager::Items::CanePrice))
					{
						Labels::getUnique("eco").getComponent<FloraManager>().createReed2(pos);
					}
				}
			}
			else if (*uib.mode == "plant2")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					if (gm->buyItem(GameManager::Items::NormalGrassPrice))
					{
						Labels::getUnique("eco").getComponent<FloraManager>().createNormalGrass(pos);
					}
				}
			}
			else if (*uib.mode == "plant3")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					if (gm->buyItem(GameManager::Items::RedGrassPrice))
					{
						Labels::getUnique("eco").getComponent<FloraManager>().createRedGrass(pos);
					}
				}
			}
			else if (*uib.mode == "plant4")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					if (gm->buyItem(GameManager::Items::ExoticGrassPrice))
					{
						Labels::getUnique("eco").getComponent<FloraManager>().createExoticGrass(pos);
					}
				}
			}
			//when you delete this, it should reassign some of the hunters to other free places
			//no it shouldnt
			else if (*uib.mode == "extra1")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					//search for all huts and assign to one that still has space
					Entity* hut = getOpenHut();
					if (hut)
					{
						if (gm->buyItem(GameManager::Items::Hunter))
						{
							auto [hutId, localId, label] = hut->getComponent<HunterHut>().addHunter();
							Factory::createHunter(hut->getComponent<HunterHut>().getPos(), hutId, localId, label);
							hut->getComponent<HunterHut>().setHutSize(hut->getComponent<HunterHut>().getHutSize() + 1);
						}
					}
				}
			}
			else if (*uib.mode == "extra2")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					target = Selector::getSelected();
					//this should only be animals
					if (target && target->getComponent<Labels>().hasLabel("animal"))
					{
						if (target->hasComponent<HerbivoreAI>() && !target->getComponent<HerbivoreAI>().getHasTracker())
						{
							if (gm->buyItem(GameManager::Items::Tracker))
							{
								Transform transform = target->getComponent<Transform>();
								target->getComponent<Light::TransformedLight>().setRadius(2.5f);
								target->getComponent<HerbivoreAI>().setHasTracker(true);
							}
						}
						else if (!target->getComponent<CarnivoreAI>().getHasTracker())
						{
							if (gm->buyItem(GameManager::Items::Tracker))
							{
								Transform transform = target->getComponent<Transform>();
								target->getComponent<Light::TransformedLight>().setRadius(2.5f);
								target->getComponent<CarnivoreAI>().setHasTracker(true);
							}
						}
					}
				}
			}
			else if (*uib.mode == "extra3")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
					if (gm->buyItem(GameManager::Items::Jeep))
					{
						Labels::getUnique("global").getComponent<GameManager>().addJeep();
					}
				}
			}
			else if (*uib.mode == "extra4")
			{
				for (Vec3 pos : getClicks(Input::MouseButton::Left)) {
						Factory::createPoacher();
				}
			}
		}
		else
		{
			//maybe will do some ui stuff idk aaaaaaaaaaaaa
			selected = Selector::getSelected();
			if (selected)
			{
				if (selected->getComponent<Labels>().hasLabel("hunter"))
				{
					hunterDirectMode = true;
				}
			}
		}
		


	}
}

Entity* EntitySelector::getOpenHut()
{
	const auto& set = Labels::getAll("hunterHut");
	std::vector<Entity*> an(set.begin(), set.end());

	for (Entity* e : an) {
		if (e->getComponent<HunterHut>().getHutSize() < 3)
		{
			return e;
		}
	}

	return nullptr;
}