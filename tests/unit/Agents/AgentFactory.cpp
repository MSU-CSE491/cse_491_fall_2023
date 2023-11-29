/**
 * @file AgentFactory.cpp
 * @author Yousif Murrani
 */

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// class project
#include "Agents/AgentFactory.hpp"
#include "Worlds/MazeWorld.hpp"

using namespace walle;

class MockWorld : public cse491::WorldBase {
    int DoAction(cse491::AgentBase &agent, size_t action_id) override {}
};

TEST_CASE("Adding an AStarAgent"){

    MockWorld world;
    AgentFactory factory(world);

    AStarAgentData agentData;
    agentData.name = "AStarAgent1";
    agentData.position = {0, 0};
    agentData.symbol = '*';
    agentData.target = {5, 5};
    agentData.recalculate_after_x_turns = 3;

    auto& agent = factory.AddAStarAgent(agentData);

    SECTION("Properties") {
        REQUIRE(agent.GetName() == agentData.name);
        REQUIRE(agent.GetPosition() == agentData.position);
        REQUIRE(agent.GetProperty<char>("symbol") == agentData.symbol);
        REQUIRE(agent.GetGoalPosition() == agentData.target);
        REQUIRE(agent.GetRecalculateValue() == agentData.recalculate_after_x_turns);
    }

    SECTION("Type") {
        REQUIRE(dynamic_cast<walle::AStarAgent*>(&agent) != nullptr);
    }
}

TEST_CASE("Adding an PacingAgent"){

}