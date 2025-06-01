#include "Health.hpp"

void Health::setHealth(int healthPoints)
{
	if (healthPoints <= 0)
	{
		logFatal("Invalid amount of HP provided");
	}
	else
	{
		currHP = healthPoints;
		isAlive = true;
	}
}

void Health::onDetach() {}

int Health::getHealth()
{
	return currHP;
}

void Health::decreaseHealth(int damage, std::string attacker)
{
	currHP -= damage;
	if (currHP <= 0)
	{
		die();
		killer = attacker;
	}
}

void Health::decreaseHealth(int damage)
{
	currHP -= damage;
	if (currHP <= 0)
	{
		die();
		killer = "";
	}
}


bool Health::getAlive()
{
	return isAlive;
}

//will probably need a more overarching implementation, it will be really important
void Health::die()
{
	isAlive = false;
}