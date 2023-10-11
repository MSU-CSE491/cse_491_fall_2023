/**
 * @file FollowingAgent.cpp
 * @author Yousif Murrani
 */


// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// class project
#include "Agents/FollowingAgent.hpp"


using namespace walle;

TEST_CASE("Following Agent Initialization", "[Agents]"){

    // created following agent
    FollowingAgent agent(1, "Following Agent");

    SECTION("Initialization Failure"){

        // check Initialize fails because agent has no actions
        REQUIRE(agent.Initialize() == false);
    }

    // added actions to following agent
    agent.AddAction("up", 1);
    agent.AddAction("down", 2);
    agent.AddAction("left", 3);
    agent.AddAction("right", 4);

    SECTION("Initialization Success"){

        // check Initialize works because agent has actions
        REQUIRE(agent.Initialize() == true);
    }

    SECTION("Agent is an Agent"){

        // agent is always an agent
        REQUIRE(agent.IsAgent() == true);
    }
}

TEST_CASE("Following Agent Position", "[Agents]"){

    // created following agent and get its position
    FollowingAgent agent(1, "Following Agent");
    cse491::GridPosition position = agent.GetPosition();

    SECTION("Default Position"){

        // check default agent position
        REQUIRE(position.GetX() == 0.0);
        REQUIRE(position.GetY() == 0.0);
    }

    SECTION("Set Position, X = Y"){

        // check that set position works
        agent.SetPosition(1.0, 1.0);
        position = agent.GetPosition();
        REQUIRE(position.GetX() == 1.0);
        REQUIRE(position.GetY() == 1.0);
    }

    SECTION("Set Position, X != Y"){

        // check that different positions can be set
        agent.SetPosition(12.0, 7.0);
        position = agent.GetPosition();
        REQUIRE(position.GetX() == 12.0);
        REQUIRE(position.GetY() == 7.0);
    }

}

TEST_CASE( "Following Agent Health", "[Agents]" ) {

    // created following agent and get its position
    FollowingAgent agent(1, "Following Agent");

    SECTION("Default Health"){

        // default following agent health
        REQUIRE(agent.GetHealth() == 20.0);
    }

    SECTION("Set Health"){

        // check that set health works
        agent.SetHealth(15.0);
        REQUIRE(agent.GetHealth() == 15.0);
        REQUIRE(agent.isAlive() == true);
    }

    SECTION("Take Damage"){

        // check that take damage works
        agent.TakeDamage(10.0);
        REQUIRE(agent.GetHealth() == 10.0);
    }

    SECTION("Killing an Agent"){

        // check that an agent dies when it takes enough damage
        agent.TakeDamage(20.0);
        REQUIRE(agent.GetHealth() == 0.0);
        REQUIRE(agent.isAlive() == false);
    }
}