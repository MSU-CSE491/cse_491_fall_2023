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
#include "core/AgentBase.hpp"
#include "Interfaces/TrashInterface.hpp"


#define REMAIN_STILL 0
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3
#define MOVE_RIGHT 4
#define DROP_ITEM 5

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

TEST_CASE("Item Test", "[World][SecondWorld]") {
    // Initialize world
    group4::SecondWorld world("../assets/grids/third_floor.grid", "../assets/third_floor_input.json");

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
    group4::SecondWorld world("../assets/grids/group4_maze.grid", "../assets/input.json");
    // REQUIRE_NOTHROW(world.LoadFromFile("../assets/input.json"));
  
    REQUIRE(world.GetNumAgents() == 3);
  }
  SECTION("Load Agents from Valid File2") {
    // group4::SecondWorld world;
    // REQUIRE_NOTHROW(world.LoadFromFile("../assets/second_floor_input.json"));
    group4::SecondWorld world("../assets/grids/second_floor.grid", "../assets/second_floor_input.json");
  
    REQUIRE(world.GetNumAgents() == 2);
  }
  SECTION("Load Agents from Valid File3") {
    // group4::SecondWorld world;
    // REQUIRE_NOTHROW(world.LoadFromFile("../assets/third_floor_input.json"));
    group4::SecondWorld world("../assets/grids/third_floor.grid", "../assets/third_floor_input.json");
  
    REQUIRE(world.GetNumAgents() == 1);
  }


}


TEST_CASE("Print Entities", "[World][SecondWorld]") {
  group4::SecondWorld world("../assets/grids/third_floor.grid", "../assets/third_floor_input.json");
  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);

  auto testItem = std::make_unique<cse491::ItemBase>(1, "Test Item");
  world.AddItem(std::move(testItem));

  // Redirect cout for testing output
  std::stringstream output;
  std::streambuf* coutBuffer = std::cout.rdbuf();
  std::cout.rdbuf(output.rdbuf());

  world.PrintEntities();

  // Reset cout buffer
  std::cout.rdbuf(coutBuffer);

  // Check the printed output after removing leading/trailing whitespaces
  std::string expectedOutput = "TestItem";
  std::string actualOutput = output.str();

  // Remove leading/trailing whitespaces
  actualOutput.erase(std::remove_if(actualOutput.begin(), actualOutput.end(), ::isspace), actualOutput.end());

  CHECK(actualOutput == expectedOutput);

  // https://chat.openai.com/share/1adb0025-b787-44f2-bd75-243533d5ae70
}


TEST_CASE("Pickup Items", "[World][SecondWorld]") {
    group4::SecondWorld world;

    auto &test_agent = world.AddAgent<cse491::TrashInterface>("Playable Character");
    test_agent.SetPosition(0, 0);

    auto testItem1 = std::make_unique<cse491::ItemBase>(1, "Test Item 1");

    testItem1->SetPosition(0, 1);
    testItem1->SetGrid();
    world.AddItem(std::move(testItem1));

    CHECK(test_agent.GetInventory().empty());

    world.DoAction(test_agent, MOVE_DOWN);
    world.DoAction(test_agent, MOVE_UP);

    // Check that item is in agent's inventory
    CHECK(test_agent.GetInventory().size() == 1);
    // Check that item is no longer on the grid
    CHECK(world.FindItemsAt(cse491::GridPosition{0, 1}).empty());
}

TEST_CASE("Drop Items", "[World][SecondWorld]") {
    group4::SecondWorld world;

    auto &test_agent = world.AddAgent<cse491::TrashInterface>("Playable Character");
    test_agent.SetPosition(0, 0);
    auto testItem1 = std::make_unique<cse491::ItemBase>(1, "Test Item 1");
    auto testItem2 = std::make_unique<cse491::ItemBase>(2, "Test Item 2");

    testItem1->SetPosition(0, 1);
    testItem1->SetGrid();
    world.AddItem(std::move(testItem1));

    testItem2->SetPosition(0, 2);
    testItem2->SetGrid();
    world.AddItem(std::move(testItem2));

    world.DoAction(test_agent, MOVE_DOWN);
    world.DoAction(test_agent, MOVE_DOWN);

    // Confirm agent has both items
    CHECK(test_agent.GetInventory().size() == 2);

    world.DoAction(test_agent, DROP_ITEM);
    // Confirm item dropped
    CHECK(test_agent.GetInventory().size() == 1);
    // And that dropping a second item onto another item doesn't work... unless it's a chest
    world.DoAction(test_agent, DROP_ITEM);
    CHECK(test_agent.GetInventory().size() == 1);

    world.DoAction(test_agent, MOVE_DOWN);
    world.DoAction(test_agent, MOVE_UP);

    // Confirm item can be picked up again after it's dropped
    CHECK(test_agent.GetInventory().size() == 2);
}


TEST_CASE("Chest", "[World][SecondWorld]") {
    group4::SecondWorld world;

    auto &test_agent = world.AddAgent<cse491::TrashInterface>("Playable Character");
    test_agent.SetPosition(0, 0);
    auto dagger = std::make_unique<cse491::ItemBase>(1, "Test Item 1");

    auto chest = std::make_unique<cse491::ItemBase>(2, "Chest");
    chest->SetPosition(0, 1);
    chest->SetProperties("symbol", 'C', "Chest", 0);
    chest->SetGrid();
    auto& temp_chest = world.AddItem(std::move(chest));

    auto& temp_dagger = world.AddItem(std::move(dagger));
    temp_chest.AddItem(temp_dagger.GetID());
    temp_dagger.SetPosition(-1, -1);

    // Check that the chest has the item in its inventory
    CHECK(temp_chest.GetInventory().size() == 1);

    world.DoAction(test_agent, MOVE_DOWN);
    // Check that the item transferred from chest to agent
    CHECK(test_agent.GetInventory().size() == 1);
    CHECK(temp_chest.GetInventory().empty());
}