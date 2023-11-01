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
