/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for ManualWorld.hpp in source/Worlds
 * @author Grant Carr
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "Worlds/ManualWorld.hpp"
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"

TEST_CASE("ManualWorld Construction", "[worlds][manual]"){
  SECTION("Default construction"){
    cse491_team8::ManualWorld world;
    world.AddAgent<cse491::TrashInterface>("Interface", "Strength", 7).SetProperty("char", '@');
    world.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 2).SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2", "Strength", 4).SetPosition(6,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 3", "Strength", 6).SetPosition(18,3);
    world.AddAgent<cse491::PacingAgent>("Pacer 4", "Strength", 8).SetPosition(21,3);
    CHECK(world.GetNumAgents() == 5);
  }
}

TEST_CASE("Finding item for agent"){
  SECTION("Successful Item Find"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 13, "Max_Health", 20);
    auto item = std::make_unique<cse491::ItemBase>(2, "Health Potion");
    item->SetProperty("Healing", 10);
    const size_t item_id = item->GetID();
    world.AddItem(std::move(item));
    agent->SetWorld(world);
    agent->AddItem(world.GetItem(item_id));
    CHECK(world.FindItem(*agent, "Health Potion") == 2);
  }

  SECTION("Item owned, finding different item"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 13, "Max_Health", 20);
    auto item = std::make_unique<cse491::ItemBase>(2, "Health Potion");
    item->SetProperty("Healing", 10);
    const size_t item_id = item->GetID();
    world.AddItem(std::move(item));
    agent->SetWorld(world);
    agent->AddItem(world.GetItem(item_id));
    CHECK(world.FindItem(*agent, "Axe") == SIZE_MAX);
  }

  SECTION("No items owned"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 13, "Max_Health", 20);
    agent->SetWorld(world);
    CHECK(world.FindItem(*agent, "Axe") == SIZE_MAX);
  }
}

TEST_CASE("Agent Healing"){
  SECTION("Attempt Heal Success Full Heal"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 5, "Max_Health", 20);
    auto item = std::make_unique<cse491::ItemBase>(2, "Health Potion");
    item->SetProperty("Healing", 10);
    const size_t item_id = item->GetID();
    world.AddItem(std::move(item));
    agent->SetWorld(world);
    agent->AddItem(world.GetItem(item_id));
    world.HealAction(*agent);
    CHECK(agent->GetProperty<int>("Health") == 15);
  }

  SECTION("Attempt Heal Success Partial Heal"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 13, "Max_Health", 20);
    auto item = std::make_unique<cse491::ItemBase>(2, "Health Potion");
    item->SetProperty("Healing", 10);
    const size_t item_id = item->GetID();
    world.AddItem(std::move(item));
    agent->SetWorld(world);
    agent->AddItem(world.GetItem(item_id));
    world.HealAction(*agent);
    CHECK(agent->GetProperty<int>("Health") == 20);
    CHECK(world.GetItem(world.GetItemID("Health Potion")).GetProperty<int>("Healing") == 3);
  }

  SECTION("No Health Potions"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 13, "Max_Health", 20);
    agent->SetWorld(world);
    world.HealAction(*agent);
    CHECK(agent->GetProperty<int>("Health") == 13);
  }
}

TEST_CASE("Agent dropping items after being defeated"){
  SECTION("Drop items"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    auto agent2 = std::make_unique<cse491::PacingAgent>(5, "Pacer 2");
    agent->SetProperties("Health", 13, "Max_Health", 20);
    auto item = std::make_unique<cse491::ItemBase>(2, "Health Potion");
    item->SetProperty("Healing", 10);
    const size_t item_id = item->GetID();
    world.AddItem(std::move(item));
    agent->SetWorld(world);
    agent2->SetWorld(world);
    agent->AddItem(item_id);
    CHECK(world.GetItem(item_id).GetOwnerID() == 1);
    world.DropItems(*agent2, *agent);
    CHECK(world.GetItem(item_id).GetOwnerID() == 0);
  }
}

TEST_CASE("Agent looking ahead"){
  SECTION("Looking above"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetWorld(world);
    agent->SetProperties("Health", 13, "Max_Health", 20, "Direction", 0);
    agent->SetPosition(5, 5);
    auto look_position = world.LookAhead(*agent);
    CHECK(look_position.GetX() == 5);
    CHECK(look_position.GetY() == 4);
  }

  SECTION("Looking below"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetWorld(world);
    agent->SetProperties("Health", 13, "Max_Health", 20, "Direction", 2);
    agent->SetPosition(5, 5);
    auto look_position = world.LookAhead(*agent);
    CHECK(look_position.GetX() == 5);
    CHECK(look_position.GetY() == 6);
  }

  SECTION("Looking left"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetWorld(world);
    agent->SetProperties("Health", 13, "Max_Health", 20, "Direction", 3);
    agent->SetPosition(5, 5);
    auto look_position = world.LookAhead(*agent);
    CHECK(look_position.GetX() == 4);
    CHECK(look_position.GetY() == 5);
  }

  SECTION("Looking right"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetWorld(world);
    agent->SetProperties("Health", 13, "Max_Health", 20, "Direction", 1);
    agent->SetPosition(5, 5);
    auto look_position = world.LookAhead(*agent);
    CHECK(look_position.GetX() == 6);
    CHECK(look_position.GetY() == 5);
  }
}