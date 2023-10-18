/**
 * @file SecondFloor.cpp
 * @author Jayson Van Dam
 */

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "Worlds/SecondFloor.hpp"

TEST_CASE("SecondFloor Construction", "[Worlds][SecondFloor]") {
    SECTION("Default construction") {
        group4::SecondFloor world;
        cse491::WorldGrid grid = world.GetGrid();
    }
}