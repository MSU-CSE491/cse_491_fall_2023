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

TEST_CASE("Item Test")
{
    // Initialize world
    group4::SecondWorld world;

    // Test adding and removing an Item  w/ properties
    auto testItem1 = std::make_unique<cse491::ItemBase>(1, "Test Item 1");
    testItem1->SetPosition(1, 2);
    testItem1->SetProperty("Damage", 20.0);

    CHECK(testItem1->GetProperty("Damage") == 20.0);

    auto testItem2 = std::make_unique<cse491::ItemBase>(2, "Test Item  2");
    testItem2->SetPosition(2, 3);
    testItem2->SetProperties("Damage", 5.5, "Fire Resistance", 2.5);

    CHECK(testItem2->GetProperty("Damage") ==  5.5);
    CHECK(testItem2->GetProperty("Fire Resistance") == 2.5);

    testItem2->RemoveProperty("Fire Resistance");
    CHECK(!testItem2->HasProperty("Fire Resistance"));

    auto & item = world.AddItem(std::move(testItem1));
    world.AddItem(std::move(testItem2));

    CHECK(world.GetNumItems() == 2);

    world.RemoveItem(item.GetID());

    CHECK(world.GetNumItems() == 1);

}
