#pragma once

class Entity;

class Component
{
public:

	//Component() = default;
	//Component(const Component &other) = delete;
	virtual ~Component() = default;

	Entity &getEntity() const;

protected:

	Entity *entity = nullptr;

	virtual void onAttach();
	virtual void onDetach();

private:

	void add(Entity &entity);
	void remove();

	bool flagAdd = false, flagRemove = false;

	friend class Core;
	friend class ComponentManager;
};
