/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/core
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "Worlds/SecondWorld.hpp"
#include "core/WorldGrid.hpp"

TEST_CASE("SecondWorld Construction", "[World][SecondWorld]"){
  SECTION("Default construction"){
    // cse491::WorldGrid grid;
    group4::SecondWorld world;
    CHECK(world.GetWidth() == 0);
    CHECK(world.GetWidth() == 0);
    CHECK(world.GetNumCells() == 0);
    CHECK(!world.IsValid(0, 0));
    CHECK(!world.IsValid(1, 1));
    CHECK(!world.IsValid(-1, -1));
  }
}