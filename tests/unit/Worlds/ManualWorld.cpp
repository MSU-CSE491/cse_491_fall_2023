/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/core
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

TEST_CASE("Agent Healing"){
  SECTION("Attempt Heal Success Full Heal"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 5, "Max_Health", 20);
    auto item = std::make_unique<cse491::ItemBase>(2, "Health Potion");
    item->SetProperty("Healing", 10);
    item->SetOwner(*agent);
    world.AddItem(std::move(item));
    world.HealAction(*agent);
    CHECK(agent->GetProperty<int>("Health") == 15);
  }

  SECTION("Attempt Heal Success Partial Heal"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 13, "Max_Health", 20);
    auto item = std::make_unique<cse491::ItemBase>(2, "Health Potion");
    item->SetProperty("Healing", 10);
    item->SetOwner(*agent);
    world.AddItem(std::move(item));
    world.HealAction(*agent);
    CHECK(agent->GetProperty<int>("Health") == 20);
    CHECK(world.GetItem(world.GetItemID("Health Potion")).GetProperty<int>("Healing") == 3);
  }

  SECTION("No Health Potions"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 13, "Max_Health", 20);
    world.HealAction(*agent);
    CHECK(agent->GetProperty<int>("Health") == 13);
  }
}

TEST_CASE("Finding item for agent"){
  SECTION("Successful Item Find"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 13, "Max_Health", 20);
    auto item = std::make_unique<cse491::ItemBase>(2, "Health Potion");
    item->SetProperty("Healing", 10);
    item->SetOwner(*agent);
    world.AddItem(std::move(item));
    CHECK(world.FindItem(*agent, "Health Potion") == 2);
  }

  SECTION("Unsuccessful Item Find"){
    cse491_team8::ManualWorld world;
    auto agent = std::make_unique<cse491::PacingAgent>(1, "Pacer");
    agent->SetProperties("Health", 13, "Max_Health", 20);
    auto item = std::make_unique<cse491::ItemBase>(2, "Health Potion");
    item->SetProperty("Healing", 10);
    item->SetOwner(*agent);
    world.AddItem(std::move(item));
    CHECK(world.FindItem(*agent, "Axe") == SIZE_T_MAX);
  }
}
