#pragma once
#include "../framework/Entity.hpp"
#include <vector>
#include "../framework/Labels.hpp"
#include "Factory.hpp"
#include "Animals/GiraffeHerd.hpp"
#include "Animals/ZebraHerd.hpp"
#include "Animals/LionPride.hpp"
#include "Animals/WildebeestHerd.hpp"
#include "AI/HerbivoreHerdAI.hpp"
#include "GameManager.hpp"
#include "FloraManager.hpp"
#include "HerdTypes.hpp"

class Seeder
{
    public:
        static void seedMain();
        static void seedMenu();
        static void resetWorld();
};