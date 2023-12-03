/**
 * @file AgentFactory.cpp
 * @author Yousif Murrani
 */

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// class project
#include <vector>
#include "Agents/AgentFactory.hpp"
#include "Agents/AStarAgent.hpp"
#include "Agents/PacingAgent.hpp"
#include "Agents/TrackingAgent.hpp"
#include "Agents/PathAgent.hpp"

using namespace walle;

/// Mock world to create factory
class MockWorld : public cse491::WorldBase {
    int DoAction(cse491::AgentBase &, size_t) override { return 0; }
};

TEST_CASE("Adding a AStarAgent", "[Agents]"){

    // create a fake world and factory
    MockWorld world;
    AgentFactory factory(world);

    // add properties/data to the factory
    AStarAgentData agentData;
    agentData.name = "AStarAgent";
    agentData.position = {0, 0};
    agentData.symbol = '*';
    agentData.target = {5, 5};
    agentData.recalculate_after_x_turns = 3;

    // factory creates correct agent
    auto& agent = factory.AddAStarAgent(agentData);

    SECTION("Properties") {
        // test properties of created agent
        REQUIRE(agent.GetName() == agentData.name);
        REQUIRE(agent.GetPosition() == agentData.position);
        REQUIRE(agent.GetProperty<char>("symbol") == agentData.symbol);
        REQUIRE(agent.GetGoalPosition() == agentData.target);
        REQUIRE(agent.GetRecalculateValue() == agentData.recalculate_after_x_turns);
    }

    SECTION("Type") {
        // test type of created agent
        REQUIRE(dynamic_cast<walle::AStarAgent*>(&agent) != nullptr);
    }
}

TEST_CASE("Adding a PacingAgent", "[Agents]"){

    // create a fake world and factory
    MockWorld world;
    AgentFactory factory(world);

    // add properties/data to the factory
    PacingAgentData agentData;
    agentData.name = "PacingAgent";
    agentData.position = {0, 0};
    agentData.symbol = '*';
    agentData.vertical = true;

    auto& agent = factory.AddPacingAgent(agentData);

    SECTION("Properties") {
        // factory creates correct agent
        REQUIRE(agent.GetName() == agentData.name);
        REQUIRE(agent.GetPosition() == agentData.position);
        REQUIRE(agent.GetProperty<char>("symbol") == agentData.symbol);
        REQUIRE(agent.GetVertical() == agentData.vertical);
    }

    SECTION("Type") {
        // test type of created agent
        REQUIRE(dynamic_cast<cse491::PacingAgent*>(&agent) != nullptr);
    }
}

TEST_CASE("Adding a TrackingAgent", "[Agents]"){

    // create a fake world and factory
    MockWorld world;
    cse491::Entity* targ = nullptr;
    AgentFactory factory(world);

    // add properties/data to the factory
    TrackingAgentData agentData;
    agentData.name = "TrackingAgent";
    agentData.position = {0, 0};
    agentData.symbol = '*';
    agentData.path = "";
    agentData.vector_path = std::vector<cse491::GridPosition>();
    agentData.target = targ;
    agentData.tracking_distance = 10;
    agentData.start_pos = {1,1};

    auto& agent = factory.AddTrackingAgent(agentData);

    SECTION("Properties") {
        // factory creates correct agent
        REQUIRE(agent.GetName() == agentData.name);
        REQUIRE(agent.GetPosition() == agentData.position);
        REQUIRE(agent.GetProperty<char>("symbol") == agentData.symbol);
    }

    SECTION("Type") {
        // test type of created agent
        REQUIRE(dynamic_cast<TrackingAgent*>(&agent) != nullptr);
    }
}

TEST_CASE("Adding a PathAgent", "[Agents]"){

    // create a fake world and factory
    MockWorld world;
    AgentFactory factory(world);

    // add properties/data to the factory
    PathAgentData agentData;
    agentData.name = "PathAgent";
    agentData.position = {0, 0};
    agentData.symbol = '*';
    // Path must not be empty
    agentData.path = "x";
    // Convert to vector
    agentData.vector_path = StrToOffsets(agentData.path);

    auto& agent = factory.AddPathAgent(agentData);

    SECTION("Properties") {
        // factory creates correct agent
        REQUIRE(agent.GetName() == agentData.name);
        REQUIRE(agent.GetPosition() == agentData.position);
        REQUIRE(agent.GetProperty<char>("symbol") == agentData.symbol);
        REQUIRE(agent.GetPath() == agentData.vector_path);
    }

    SECTION("Type") {
        // test type of created agent
        REQUIRE(dynamic_cast<PathAgent*>(&agent) != nullptr);
    }
}