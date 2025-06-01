#pragma once

#include "Behaviour.hpp"

class Decoration : public Behaviour {

public:

    // ------------------------------
    // | Constructors
    // ------------------------------

    Decoration();


    // ------------------------------
    // | Public methods
    // ------------------------------

    // (..)


    // ------------------------------
    // | Overrides
    // ------------------------------

    void start(Entity& entity) override;
    void update(Entity& entity) override;


    // ------------------------------
    // | Getters/setters
    // ------------------------------



private:

    // ------------------------------
    // | Private methods
    // ------------------------------

    // (..)


    // ------------------------------
    // | Fields
    // ------------------------------



};

