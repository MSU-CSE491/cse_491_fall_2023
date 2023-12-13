/**
 * @file TestPathAgent.cpp
 * @author David Rackerby
 */

// Catch2
#define CATCH_CONFIG_MAIN

#include "Agents/PathAgent.hpp"

#include <catch2/catch_all.hpp>
#include <ranges>
#include <utility>

#include "Worlds/MazeWorld.hpp"

using namespace std;

TEST_CASE("Path Agent Constructor", "[Agents]")
{
  SECTION("Constructor throws on empty path")
  {
    // Internal path vector must be indexable from the start

    vector<cse491::GridPosition> empty;
    REQUIRE_THROWS(walle::PathAgent(0, "Zeroth", std::move(empty)));
    REQUIRE_THROWS(walle::PathAgent(0, "Zeroth", ""));

    // Should ignore extra whitespace
    REQUIRE_THROWS(walle::PathAgent(0, "Zeroth", "             "));

    // But a path where the agent stays put is okay
    REQUIRE_NOTHROW(walle::PathAgent(0, "Zeroth", "x"));
  }
}

TEST_CASE("Path Agent String Helper", "[Agents]")
{
  SECTION("Basic string")
  {
    auto vec = walle::StrToOffsets("n n n");
    vector<cse491::GridPosition> expected(3, {0, -1});

    // If we could use C++23 std::ranges::views::zip, this would be cleaner
    for (int i = 0; i < static_cast<int>(expected.size()); ++i) {
      REQUIRE(vec[i] == expected[i]);
    }

    vec = walle::StrToOffsets("3n");
    for (int i = 0; i < static_cast<int>(expected.size()); ++i) {
      REQUIRE(vec[i] == expected[i]);
    }

    vec = walle::StrToOffsets("3*n");
    expected = {{0, -3}};
    REQUIRE(vec[0] == expected[0]);
  }

  SECTION("Documentation example")
  {
    auto vec = walle::StrToOffsets("n w 3e 10*s 5*w x");
    vector<cse491::GridPosition> expected = {{0, -1}, {-1, 0}, {1, 0},  {1, 0},
                                             {1, 0},  {0, 10}, {-5, 0}, {0, 0}};
    for (int i = 0; i < static_cast<int>(expected.size()); ++i) {
      REQUIRE(vec[i] == expected[i]);
    }
  }
}

TEST_CASE("Path Agent Accessors", "[Agents]")
{
  SECTION("Single-pass")
  {
    // 6 commands
    walle::PathAgent dummy(0, "Dummy", "x x x x x x");
    array<int, 6> indices = {0, 1, 2, 3, 4, 5};

    for (auto const& i : indices) {
      REQUIRE(dummy.GetIndex() == i);
      dummy.IncrementIndex();
    }
  }

  SECTION("Loop forward")
  {
    // 6 commands, loops over them in order twice
    walle::PathAgent dummy(0, "Dummy", "x x x x x x");
    array<int, 13> indices = {0, 1, 2, 3, 4, 5, 0, 1, 2, 3, 4, 5, 0};

    for (auto const& i : indices) {
      REQUIRE(dummy.GetIndex() == i);
      dummy.IncrementIndex();
    }
  }

  SECTION("Loop backward")
  {
    // 6 commands, loops over them in reverse-order twice
    walle::PathAgent dummy(0, "Dummy", "x x x x x x");
    array<int, 13> indices = {0, 5, 4, 3, 2, 1, 0, 5, 4, 3, 2, 1, 0};

    for (auto const& i : indices) {
      REQUIRE(dummy.GetIndex() == i);
      dummy.DecrementIndex();
    }
  }
}

TEST_CASE("Path Agent Setters", "[Agents]")
{
  SECTION("Reset path")
  {
    // Setup
    walle::PathAgent agent(0, "Reset", "n n n");
    auto next_pos = agent.GetNextPosition();
    agent.SetPosition(next_pos);

    REQUIRE(agent.GetPosition() == cse491::GridPosition(0, -1));
    REQUIRE(agent.GetIndex() == 1);

    // Reset both offsets and index
    agent.SetPath("x", 0);
    REQUIRE(agent.GetPath()[0] == cse491::GridPosition(0, 0));
    REQUIRE(agent.GetIndex() == 0);

    // Start index is >= the number of offsets
    REQUIRE_THROWS(agent.SetPath("x", 1));
  }
}

TEST_CASE("Path Agent Behavior", "[Agents]")
{
  SECTION("Basic")
  {
    vector<cse491::GridPosition> offsets = {{0, 1}};
    walle::PathAgent agent(0, "First", std::move(offsets));
    auto next_pos = agent.GetNextPosition();
    agent.SetPosition(next_pos);
    REQUIRE(agent.GetPosition() == cse491::GridPosition(0, 1));
  }

  SECTION("Pacing up and down")
  {
    walle::PathAgent agent(0, "Second", "n s");
    vector<cse491::GridPosition> expected = {{0, -1}, {0, 0}, {0, -1}, {0, 0}};
    for (auto const& expected_pos : expected) {
      auto next_pos = agent.GetNextPosition();
      agent.SetPosition(next_pos);
      REQUIRE(agent.GetPosition() == expected_pos);
    }
  }
}

TEST_CASE("Agent State")
{
  // initialize agent and world
  walle::PathAgent agent(1, "TestAgent");
  cse491::MazeWorld world;
  agent.SetWorld(world);
  agent.SetPosition(cse491::GridPosition(0, 0));

  // give agent properties
  agent.SetProperty("Health", 10);
  agent.SetProperty("Taking_Damage", false);
  agent.SetProperty("Max_Health", 15);

  // agent is healthy
  agent.UpdateAgentState(agent);
  SECTION("Healthy State") { REQUIRE(agent.GetAgentState() == cse491::Healthy); }

  // agent is taking damage
  agent.SetProperty("Taking_Damage", true);
  agent.UpdateAgentState(agent);
  SECTION("Taking Damage State") { REQUIRE(agent.GetAgentState() == cse491::Taking_Damage); }

  // agent is dying
  agent.SetProperty("Taking_Damage", false);
  agent.SetProperty("Health", 3);
  agent.UpdateAgentState(agent);
  SECTION("Dying State") { REQUIRE(agent.GetAgentState() == cse491::Dying); }

  // agent is dead
  agent.SetProperty("Health", 0);
  agent.UpdateAgentState(agent);
  SECTION("Deceased State") { REQUIRE(agent.GetAgentState() == cse491::Deceased); }
}