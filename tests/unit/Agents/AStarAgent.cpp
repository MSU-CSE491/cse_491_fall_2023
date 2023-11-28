/**
 * @file AStarAgent.cpp
 * @author Yousif Murrani
 */

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// class project
#include "Agents/AStarAgent.hpp"
#include "Worlds/MazeWorld.hpp"

using namespace walle;

TEST_CASE("AStar Agent Initialization", "[Agents]") {

    // create a star agent
    AStarAgent agent(1, "TestAgent");

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

TEST_CASE("AStar Agent SetWorld", "[Agents]"){
    AStarAgent agent(1, "TestAgent");
    cse491::MazeWorld world;

    SECTION("No World"){

        // world has not been set yet
        REQUIRE(agent.HasWorld() == false);
    }

    agent.SetWorld(world);

    SECTION("Yes World"){

        // world has been set
        REQUIRE(agent.HasWorld() == true);
    }
}

TEST_CASE("AStarAgent SetGoalPosition", "[Agents]") {

    AStarAgent agent(1, "TestAgent");
    agent.SetGoalPosition(10.0, 20.0);

    SECTION("Set Goal 1"){

        // check the x and y
        REQUIRE(agent.GetGoalPosition().GetX() == 10.0);
        REQUIRE(agent.GetGoalPosition().GetY() == 20.0);
    }

    agent.SetGoalPosition(2.0, 103.0);

    SECTION("Set Goal 2"){

        REQUIRE(agent.GetGoalPosition().GetX() == 2.0);
        REQUIRE(agent.GetGoalPosition().GetY() == 103.0);
    }
}

TEST_CASE("AStarAgent SetRecalculate", "[Agents]") {
    AStarAgent agent(1, "TestAgent");

    SECTION("Default Recalculate Value"){

        // default value
        REQUIRE(agent.GetRecalculateValue() == 100);
    }

    agent.SetRecalculate(50);

    SECTION("Set Recalculate Value"){

        // new value
        REQUIRE(agent.GetRecalculateValue() == 50);
    }

}

TEST_CASE("AStarAgent RecalculatePath", "[Agents]") {
    AStarAgent agent(1, "TestAgent");
    cse491::MazeWorld world;
    agent.SetWorld(world);
    agent.SetPosition(cse491::GridPosition(0, 0));

	SECTION("Invalid Goal Position"){

		// check that the agent can't find a path to an invalid location
		agent.SetGoalPosition(0.0, 5.0);
		agent.RecalculatePath();

		REQUIRE(agent.GetPathLength() == 0);

	}

	SECTION("Valid Goal Position")
	{

		// check that the agents calculated path is > 0 when the goal position is correct
		agent.SetGoalPosition(0.0, 6.0);
		agent.RecalculatePath();

		REQUIRE(agent.GetPathLength() > 0);
	}
}

TEST_CASE("AStarAgent SelectAction", "[Agents]") {
    AStarAgent agent(1, "TestAgent");
    cse491::MazeWorld world;
    agent.SetWorld(world);
    agent.SetPosition(cse491::GridPosition(0, 0));
    agent.SetGoalPosition(10.0, 10.0);
    agent.RecalculatePath(); // Ensure a path is available

    cse491::WorldGrid grid;
    cse491::type_options_t type_options;
    cse491::item_map_t item_set;
    cse491::agent_map_t agent_set;

    size_t action = agent.SelectAction(grid, type_options, item_set, agent_set);

    // You should test if the selected action is correct based on the agent's current position and goal
    // For example, if the agent's position is (0, 0) and the goal is (10, 10), the action should be "up"
    // Ensure the action map is set correctly for this test
    REQUIRE(action == agent.HasAction("up"));
}




