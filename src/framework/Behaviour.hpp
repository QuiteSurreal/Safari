#pragma once

#include "Utils.hpp"
#include "Entity.hpp"
#include "Component.hpp"

class Behaviour : virtual public Component
{
protected:

	virtual void start(Entity &entity);
	virtual void earlyUpdate(Entity &entity);
	virtual void update(Entity &entity);
	virtual void lateUpdate(Entity &entity);

private:

	bool started = false;

	static void earlyUpdateAll();
	static void updateAll();
	static void lateUpdateAll();

	friend class Core;
};
