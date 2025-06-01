#pragma once

#include <queue>
#include <vector>
#include <algorithm>
#include <random>
#include "Behaviour.hpp"
#include "Core.hpp"
#include "Sprite.hpp"
#include "Utils.hpp"
#include "Input.hpp"




class Animator : public Behaviour {

public:

    // ------------------------------
    // | Constructors
    // ------------------------------

    Animator();


    // ------------------------------
    // | Public methods
    // ------------------------------

    void addAnimation(std::string name, std::vector<std::string> frames);
    bool removeAnimation(std::string name);
    bool switchAnimation(std::string name);
    void resetFrame();
    void resetFrameTimer();


    // ------------------------------
    // | Overrides                  
    // ------------------------------

    void start(Entity& entity) override;
    void update(Entity& entity) override;
    

    // ------------------------------
    // | Getters/setters            
    // ------------------------------

    bool isActive() const { return active; }
    void setActive(bool value) { active = value; }

    float getDelay() const { return delay; }
    void setDelay(float value) { delay = value; }

    
private:

    // ------------------------------
    // | Private methods
    // ------------------------------
    
    std::vector<Texture> texturesFromSources(std::vector<std::string> sources);


    // ------------------------------
    // | Fields
    // ------------------------------

    // --- timers ---

    float accumulator;
    float timeStep;
    float every6FrameTimer;

    // ---

    bool active;
    float delay;
    float timerValue;
    std::string currentAnimation;
    std::unordered_map<std::string, std::vector<Texture>> animations;
    std::unordered_map<std::string, int> currentFrame;

};

