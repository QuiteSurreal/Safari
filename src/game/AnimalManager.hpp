#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <random>
#include "Behaviour.hpp"
#include "Labels.hpp"
#include "Health.hpp"
#include "Factory.hpp"
#include "Transform.hpp"
#include "Core.hpp"
#include "AI/HerbivoreHerdAI.hpp"
#include "AI/HerbivoreAI.hpp"
#include "DestinationBasedMovement/DestinationBasedMovement.hpp"


#include "Factory.hpp"
#include "Core.hpp"
#include "Background.hpp"
#include "Time.hpp"
#include "UI.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Labels.hpp"
#include "Targetable.hpp"
#include "FaceCamera.hpp"
#include "UIBehaviour.hpp"
#include "Cow.hpp"
#include "Grid.hpp"
#include "AI/Sexes.hpp"
#include "Sprite.hpp"
#include "InteractableObjects/Poacher.hpp"
#include "DestinationBasedMovement/DestinationBasedMovement.hpp"
#include "./Animals/Zebra.hpp"
#include "./Animals/ZebraHerd.hpp"
#include "./TestBehaviours/AoECow.hpp"
#include "./Animals/RandomMigration.hpp"
#include "./Animals/Wildebeest.hpp"
#include "./Animals/WildebeestHerd.hpp"
#include "./Animals/Giraffe.hpp"
#include "./Animals/GiraffeHerd.hpp"
#include "./Animals/Lion.hpp"
#include "./Animals/LionPride.hpp"

#include "./AI/HerbivoreAI.hpp"
#include "./AI/HerbivoreHerdAI.hpp"

#include "GridSelector.hpp"
#include "InteractableObjects/EntitySelector.hpp"
#include "Road.hpp"
#include "Texture.hpp"
#include "Health.hpp"
#include "InteractableObjects/DroneStation.hpp"
#include "InteractableObjects/Drone.hpp"
#include "InteractableObjects/Airship.hpp"
#include "InteractableObjects/SecurityCamera.hpp"
#include "InteractableObjects/HunterHut.hpp"
#include "InteractableObjects/Hunter.hpp"
#include "Selector.hpp"
#include "Animator.hpp"

// DELETE THOS WE DONT NEED




class AnimalManager : public Behaviour {

public:

    // ------------------------------
    // | Constructors
    // ------------------------------

    AnimalManager();


    // ------------------------------
    // | Public methods
    // ------------------------------

    Entity& createZebra(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex);
    Entity& createWildebeest(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex);
    Entity& createGiraffe(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex);
    Entity& createRhino(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex);
    Entity& createLion(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex);
    Entity& createHyena(int herdId, int localId, const std::string& herdLabel, const std::string& animalLabel, Sexes sex);

    // Won't need these!!!!!!
    Entity& createLoveMarker(const Vec3& pos);
    Entity& createLoveMarker(Transform& reference);
    // aughh
    
    // ------------------------------
    // | Overrides                  
    // ------------------------------

    // ..
    

    // ------------------------------
    // | Getters/setters            
    // ------------------------------

    float getGlobalAnimalSpeedCoefficient() const { return globalAnimalSpeedCoefficient; }
    void setGlobalAnimalSpeedCoefficient(float value) { globalAnimalSpeedCoefficient = value; }

    float getDefSpookPropagationRadius() const { return defSpookPropagationRadius; }
    void setDefSpookPropagationRadius(float value) { defSpookPropagationRadius = value; }

    float getDefIdleTimeBaseline() const { return defIdleTimeBaseline; }
    void setDefIdleTimeBaseline(float value) { defIdleTimeBaseline = value; }

    float getDefIdleTimeFluctuation() const { return defIdleTimeFluctuation; }
    void setDefIdleTimeFluctuation(float value) { defIdleTimeFluctuation = value; }

    float getDefSpookTimeBaseline() const { return defSpookTimeBaseline; }
    void setDefSpookTimeBaseline(float value) { defSpookTimeBaseline = value; }

    float getDefSpookTimeFluctuation() const { return defSpookTimeFluctuation; }
    void setDefSpookTimeFluctuation(float value) { defSpookTimeFluctuation = value; }
    

private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    // ..


    // ------------------------------
    // | Fields
    // ------------------------------    

    // --- Should not change during runtime
    float globalAnimalSpeedCoefficient;
    float defSpookPropagationRadius;
    float defIdleTimeBaseline;
    float defIdleTimeFluctuation;
    float defSpookTimeBaseline;
    float defSpookTimeFluctuation;


};

