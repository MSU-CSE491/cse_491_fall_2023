/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for Data.hpp in source/Agents
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// class project
#include "Agents/FollowingAgent.hpp"


using namespace walle;

TEST_CASE( "Following Agent", "[agent]" ) {

	// created following agent
    FollowingAgent agent1(1, "Following Agent 1");

	SECTION("Agent Initialization"){

		// added actions to following agent
		agent1.AddAction("up", 1);
		agent1.AddAction("down", 2);
		agent1.AddAction("left", 3);
		agent1.AddAction("right", 4);

		// check Initialize and IsAgent functions
		REQUIRE(agent1.Initialize() == true);
		REQUIRE(agent1.IsAgent() == true);
	}

	SECTION("Agent Position"){
		GridPosition position = agent1.GetPosition();

		// check default agent position
		REQUIRE(position.GetX() == 0.0);
		REQUIRE(position.GetY() == 0.0);

		// check that set position works
		agent1.SetPosition(1.0, 1.0);
		position = agent1.GetPosition();
		REQUIRE(position.GetX() == 1.0);
		REQUIRE(position.GetY() == 1.0);

		agent1.SetPosition(12.0, 7.0);
		position = agent1.GetPosition();
		REQUIRE(position.GetX() == 12.0);
		REQUIRE(position.GetY() == 7.0);
	}

	SECTION("Agent Health"){

		// default following agent health
		REQUIRE(agent1.GetHealth() == 20.0);

		// check that set health works
		agent1.SetHealth(15.0);
		REQUIRE(agent1.GetHealth() == 15.0);
		REQUIRE(agent1.isAlive() == true);

		// check that take damage works
		agent1.TakeDamage(5.0);
		REQUIRE(agent1.GetHealth() == 10.0);

		// check that an agent dies when it takes enough damage
		agent1.TakeDamage(10.0);
		REQUIRE(agent1.GetHealth() == 0.0);
		REQUIRE(agent1.isAlive() == false);
	}

}