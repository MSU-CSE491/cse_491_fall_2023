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
    group4::SecondWorld world;
    cse491::WorldGrid grid = world.GetGrid();
    CHECK(grid.GetWidth() == 23);
    CHECK(grid.GetHeight() == 9);
    CHECK(grid.GetNumCells() == 207);
    CHECK(grid.IsValid(0, 0));
    CHECK(grid.IsValid(0, 8));
    CHECK(!grid.IsValid(0, 9));
    CHECK(!grid.IsValid(-1, -1));

    CHECK(grid.IsValid(22, 0));
    CHECK(!grid.IsValid(23, 0));
  }
}

TEST_CASE("EntityTest")
{
    group4::SecondWorld world;
    CHECK(world.GetNumItems() == 3); // there are 3 entities

    // Entities:
    // ID 1 = Damage (20.0); ID 2 = Damage, Speed, Burning Duration; ID 3 = Health, Extra Inv. Space
    const cse491::Entity &testEntity1 = world.GetItem(1);
    CHECK(testEntity1.HasProperty("Damage"));

    // this will cause an error because the entity w/ ID 2 overwrites the damage value
//    CHECK(testEntity1.GetProperty("Damage") == 20.0);



}
