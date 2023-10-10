/**
 * @file TestPathAgent.cpp
 * @author David Rackerby
 */

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "Agents/PathAgent.hpp"

TEST_CASE("Path Agent", "[Agents]"){
  SECTION("Basic") {
    std::vector<cse491::GridPosition> offsets = {{0, 1}};
    walle::PathAgent agent (0, "First", std::move(offsets));
    agent.Update();
    REQUIRE(agent.GetPosition() == cse491::GridPosition(0, 1));
  }

  SECTION("Pacing up and down") {
    walle::PathAgent agent(0, "Second", "n s");
    std::vector<cse491::GridPosition> expected = {{0, -1}, {0, 0}, {0, -1}, {0, 0}};
    for (auto const& expected_pos : expected) {
      agent.Update();
      REQUIRE(agent.GetPosition() == expected_pos);
    }
  }
}
