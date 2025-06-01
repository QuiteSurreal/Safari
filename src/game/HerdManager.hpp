#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <random>
#include "Behaviour.hpp"
#include "Labels.hpp"
#include "Health.hpp"
#include "Factory.hpp"
#include "Core.hpp"
#include "AI/HerbivoreHerdAI.hpp"
#include "AI/HerbivoreAI.hpp"
#include "AI/HerbivoreSiteMemory.hpp"
#include "AI/CarnivorePackAI.hpp"
#include "AI/CarnivoreAI.hpp"
#include "DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "AnimalManager.hpp"
#include "AI/Sexes.hpp"
#include "HerdTypes.hpp"




class HerdManager : public Behaviour {

public:

    /*enum HerdTypes {
        Zebra,
        Wildebeest,
        Giraffe,
        Rhino,
        Lion,
        Hyena
    };*/


    // ------------------------------
    // | Constructors
    // ------------------------------

    HerdManager();


    // ------------------------------
    // | Public methods
    // ------------------------------

    void createZebraHerd(Vec3 startPos, int initialSize);
    void createWildebeestHerd(Vec3 startPos, int initialSize);
    void createLionPride(Vec3 startPos, int initialSize);
    void createHyenaPack(Vec3 startPos, int initialSize);

    Entity& addAnimalToHerd(HerdTypes type, Sexes sex, int id);

    int getNextHerdId(HerdTypes type);


    // ------------------------------
    // | Overrides                  
    // ------------------------------

    // ..
    

    // ------------------------------
    // | Getters/setters            
    // ------------------------------

    std::unordered_map<HerdTypes, std::vector<Entity*>> getHerdMap() { return herdMap; }

private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    


    // ------------------------------
    // | Fields
    // ------------------------------

    std::unordered_map<HerdTypes, std::vector<Entity*>> herdMap;


};

