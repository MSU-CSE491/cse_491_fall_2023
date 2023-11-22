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

    // create random agent
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

TEST_CASE("Random Variable", "[Agents]"){

    RandomAgent agent(1, "TestAgent");

    agent.AddAction("up", 1);
    agent.AddAction("down", 2);
    agent.AddAction("left", 3);
    agent.AddAction("right", 4);

    // has actions so agent is initialized
    REQUIRE(agent.Initialize() == true);

    SECTION("Initial Random"){
        // initial random variable
        REQUIRE(agent.GetRandom() == 4.0);
    }

    SECTION("Calculate Random") {
        // check random values multiple times
        agent.CalculateRandom(4);
        REQUIRE(agent.GetRandom() >= 0.0);
        REQUIRE(agent.GetRandom() < 4.0);
    }

    SECTION("Set Random"){
        // check setting random value
        agent.SetDirection(1);
        REQUIRE(agent.GetRandom() == 1.0);
    }
}

TEST_CASE("Random Agent Moving", "[Agents]"){

    RandomAgent agent(1, "TestAgent");

    agent.AddAction("up", 1);
    agent.AddAction("down", 2);
    agent.AddAction("left", 3);
    agent.AddAction("right", 4);

    // has actions so agent is initialized
    REQUIRE(agent.Initialize() == true);

    SECTION("Initial Moving"){
        REQUIRE(agent.GetMoving() ==true);
    }

    // set moving and check that it worked
    agent.SetMoving(false);

    SECTION("Set Moving"){
        REQUIRE(agent.GetMoving() == false);
    }
}

//TEST_CASE("Random Agent Select Actions", "[Agents]") {
//
//    RandomAgent agent(1, "TestAgent");
//
//    agent.AddAction("up", 1);
//    agent.AddAction("down", 2);
//    agent.AddAction("left", 3);
//    agent.AddAction("right", 4);
//
//    // has actions so it is initialized
//    REQUIRE(agent.Initialize() == true);
//
//    SECTION("Agent Moving") {
//        // set moving true
//        agent.SetMoving(true);
//
//        // select action many times to ensure it's within the valid range
//        for (int i = 0; i < 100; ++i) {
//            size_t selectedAction = agent.SelectAction({}, {}, {}, {});
//            REQUIRE(selectedAction >= 1);
//            REQUIRE(selectedAction <= 4);
//        }
//    }
//
//    SECTION("Agent Not Moving") {
//        // set moving false
//        agent.SetMoving(false);
//
//        // select action returns 0
//        for (int i = 0; i < 100; ++i) {
//            size_t selectedAction = agent.SelectAction({}, {}, {}, {});
//            REQUIRE(selectedAction == 0);
//        }
//    }
//}