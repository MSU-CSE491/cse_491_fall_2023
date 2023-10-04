/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/Agents
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// class project
#include "../Agents/FollowingAgent.hpp"


using namespace walle;

TEST_CASE( "Following Agent", "[agent]" ) {

    FollowingAgent agent1(1, "Following Agent 1");
    CHECK(agent1.IsAgent() == true);

    // Check setting position
    agent1.SetPosition(1.0, 1.0);
    CHECK( agent1.GetPosition().GetX() == 1.0 );
    CHECK( agent1.GetPosition().GetY() == 1.0 );

    // Check setting health, taking damage and isAlive
    agent1.SetHealth(15);
    CHECK(agent1.GetHealth() == 15);
    CHECK(agent1.isAlive() == true);

    agent1.TakeDamage(5);
    CHECK(agent1.GetHealth() == 10);

    agent1.TakeDamage(10);
    CHECK(agent1.isAlive() == false);

    // Check the action maps
    CHECK(agent1.HasAction("up") == true);
    CHECK(agent1.HasAction("down") == true);
    CHECK(agent1.HasAction("left") == true);
    CHECK(agent1.HasAction("right") == true);

}