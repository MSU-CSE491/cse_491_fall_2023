/**
 * @file RandomAgent.cpp
 * @author Yousif Murrani
 */

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// class project
#include "Agents/RandomAgent.hpp"
#include "Worlds/MazeWorld.hpp"

using namespace walle;

TEST_CASE("Random Agent Initialization", "[Agents]") {

    // create a star agent
    RandomAgent agent(1, "TestAgent");

    SECTION("Initialization Failure"){

        // no actions so false
        REQUIRE(agent.Initialize() == false);
    }

    agent.AddAction("up", 1);
    agent.AddAction("down", 2);
    agent.AddAction("left", 3);
    agent.AddAction("right", 4);

    SECTION("Initialization Success"){

        // has actions so agent is initialized
        REQUIRE(agent.Initialize() == true);
    }
}