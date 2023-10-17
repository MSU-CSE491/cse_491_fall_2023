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
    // Initialize world
    group4::SecondWorld world;

    // Test adding and removing an Entity w/ properties
    auto testEntity1 = std::make_unique<cse491::Entity>(1, "Test Entity 1");
    testEntity1->SetPosition(1, 2);
    testEntity1->SetProperty("Damage", 20.0);


    CHECK(testEntity1->GetProperty("Damage") == 20.0);


    auto testEntity2 = std::make_unique<cse491::Entity>(2, "Test Entity 2");
    testEntity2->SetPosition(2, 3);
    testEntity2->SetProperties("Damage", 5.5, "Fire Resistance", 2.5);

    CHECK(testEntity2->GetProperty("Damage") ==  5.5);
    CHECK(testEntity2->GetProperty("Fire Resistance") == 2.5);

    auto entityID1 = world.AddEntity(testEntity1);
    auto entityID2 = world.AddEntity(testEntity2);

    CHECK(world.GetNumItems() == 2);

    world.RemoveEntity(entityID1);
    CHECK(world.GetNumItems() == 1);

}
