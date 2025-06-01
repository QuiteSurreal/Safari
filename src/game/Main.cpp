#include "Core.hpp"
#include "Factory.hpp"
#include "../game/Animals/ZebraHerd.hpp"
#include "../game/Animals/WildebeestHerd.hpp"
#include "../game/Animals/GiraffeHerd.hpp"
#include "../game/Animals/LionPride.hpp"
#include "../game/AI/HerbivoreHerdAI.hpp"
#include "GameManager.hpp"
#include "Seeder.hpp"




int main()
{
	Core::initialize();

	Factory::createGlobal();
	Core::loop();
}
