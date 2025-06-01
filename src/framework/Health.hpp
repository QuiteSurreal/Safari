#pragma once

#include "Utils.hpp"
#include "Component.hpp"



class Health : virtual public Component
{
public:
	void setHealth(int healthPoints);
	void onDetach() override;

	int getHealth();

	void decreaseHealth(int damage, std::string attacker);
	void decreaseHealth(int damage);

	bool getAlive();

	std::string getKiller()
	{
		return Health::killer;
	}

private:
	void die();

	int currHP;

	bool isAlive;
	std::string killer;

};