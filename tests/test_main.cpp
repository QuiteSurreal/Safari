// test.cpp

#include "pch.h"
#include <gtest/gtest.h>
#include "Road.hpp"
#include "Entity.hpp"
#include "Core.hpp"
#include <Factory.hpp>
#include <Animals/LionPride.hpp>
#include <AI/HerbivoreHerdAI.hpp>
#include <Animals/GiraffeHerd.hpp>
#include <Animals/WildebeestHerd.hpp>
#include <Animals/ZebraHerd.hpp>


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    Core::initialize();
	Factory::createGlobal();
    return RUN_ALL_TESTS();
}
