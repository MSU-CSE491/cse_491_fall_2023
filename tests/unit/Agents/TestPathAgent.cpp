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
    walle::PathAgent pa{0, "agent_1", "nswex"};

  }

}
