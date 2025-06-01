#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <random>
#include "Behaviour.hpp"
#include "Labels.hpp"
#include "Factory.hpp"
#include "Core.hpp"
#include "Transform.hpp"
#include "EdiblePlantTypes.hpp"
#include "EdiblePlants/EdiblePlant.hpp"
#include "Grid.hpp"
#include "FaceCamera.hpp"
#include "Animator.hpp"
#include "Targetable.hpp"
#include "Sprite.hpp"
#include "Input.hpp"



class FloraManager : public Behaviour {

public:

    // ------------------------------
    // | Constructors
    // ------------------------------

    FloraManager();


    // ------------------------------
    // | Public methods
    // ------------------------------

    Entity& createNormalGrass(Vec3 startPos);
    Entity& createThickGrass(Vec3 startPos);
    Entity& createRedGrass(Vec3 startPos);
    Entity& createExoticGrass(Vec3 startPos);
    Entity& createEdibleTree(Vec3 startPos);
    Entity& createReed1(Vec3 startPos);
    Entity& createReed2(Vec3 startPos);
    void setInitialPlantCount(EdiblePlantTypes plantType, int count);
    int getInitialPlantCount(EdiblePlantTypes plantType) const;
    void generateInitialPlants(float clumpiness);
    void generateInitialWaterPlants(float clumpiness, const std::unordered_map<EdiblePlantTypes, int>& waterPlants);


    // ------------------------------
    // | Overrides                  
    // ------------------------------

    void update(Entity& entity) override;
    

    // ------------------------------
    // | Getters/setters            
    // ------------------------------

    // ..
    

private:

    // ------------------------------
    // | Private methods
    // ------------------------------
    
    // ..


    // ------------------------------
    // | Fields
    // ------------------------------

    std::unordered_map<EdiblePlantTypes, std::vector<Entity*>> plantMap;
    std::unordered_map<EdiblePlantTypes, int> initialPlantCounts;


};

