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

  SECTION("World with second_floor grid") {
    group4::SecondWorld world("../assets/grids/second_floor.grid", "../assets/second_floor_input.json");
    cse491::WorldGrid grid = world.GetGrid();
    CHECK(grid.GetWidth() == 50);
    CHECK(grid.GetHeight() == 44);
    CHECK(grid.GetNumCells() == 2200);
    CHECK(grid.IsValid(0, 0));
    CHECK(grid.IsValid(0, 43));
    CHECK(!grid.IsValid(0, 44));
    CHECK(!grid.IsValid(-1, -1));

    CHECK(grid.IsValid(49, 0));
    CHECK(!grid.IsValid(50, 0));
  }

  SECTION("World with third_floor grid") {
    group4::SecondWorld world("../assets/grids/third_floor.grid", "../assets/third_floor_input.json");
    cse491::WorldGrid grid = world.GetGrid();
    CHECK(grid.GetWidth() == 20);
    CHECK(grid.GetHeight() == 20);
    CHECK(grid.GetNumCells() == 400);
    CHECK(grid.IsValid(0, 0));
    CHECK(grid.IsValid(0, 19));
    CHECK(!grid.IsValid(0, 20));
    CHECK(!grid.IsValid(-1, -1));

    CHECK(grid.IsValid(19, 0));
    CHECK(!grid.IsValid(20, 0));
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

TEST_CASE("Third floor agents") {
  group4::SecondWorld world("../assets/grids/third_floor.grid", "../assets/third_floor_input.json");
  CHECK(world.GetNumAgents() == 1);
  std::vector<size_t> agents = world.FindAgentsAt(cse491::GridPosition(7.0, 8.0), 0);
  CHECK(agents.size() == 1);
}


TEST_CASE("SecondWorld LoadFromFile", "[World][SecondWorld]") {
  // Test loading agents from a JSON file
  SECTION("Load Agents from Valid File") {
    group4::SecondWorld world;
    REQUIRE_NOTHROW(world.LoadFromFile("../assets/input.json"));
  
    REQUIRE(world.GetNumAgents() == 3);
  }
  SECTION("Load Agents from Valid File2") {
    group4::SecondWorld world;
    REQUIRE_NOTHROW(world.LoadFromFile("../assets/second_floor_input.json"));
  
    REQUIRE(world.GetNumAgents() == 2);
  }
  SECTION("Load Agents from Valid File3") {
    group4::SecondWorld world;
    REQUIRE_NOTHROW(world.LoadFromFile("../assets/third_floor_input.json"));
  
    REQUIRE(world.GetNumAgents() == 1);
  }


}