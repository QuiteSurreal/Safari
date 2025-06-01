#include "Behaviour.hpp"
#include "Entity.hpp"
#include "ComponentManager.hpp"

void Behaviour::start(Entity &)
{
}

void Behaviour::earlyUpdate(Entity &)
{
}

void Behaviour::update(Entity &)
{
}

void Behaviour::lateUpdate(Entity &)
{
}

void Behaviour::earlyUpdateAll()
{
	ComponentManager::visitComponents<Behaviour>([](Behaviour &component) {
		if (!component.started) {
			component.start(component.getEntity());
			component.started = true;
		}		
		component.earlyUpdate(component.getEntity());
	});
}

void Behaviour::updateAll()
{
	ComponentManager::visitComponents<Behaviour>([](Behaviour &component) {
		component.update(component.getEntity());
	});
}

void Behaviour::lateUpdateAll()
{
	ComponentManager::visitComponents<Behaviour>([](Behaviour &component) {
		component.lateUpdate(component.getEntity());
	});
}
