/**
 * @file AgentFactory.cpp
 * @author Yousif Murrani, David Rackerby
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

TEST_CASE("Adding a AStarAgent", "[Agents]") {

  // create a fake world and factory
  MockWorld world;
  AgentFactory factory(world);

  // add properties/data to the factory
  AStarAgentData agentData;
  agentData.name = "AStarAgent";
  agentData.position = {0, 0};
  agentData.symbol = '*';
  agentData.goal_pos = {5, 5};
  agentData.recalculate_after_x_turns = 3;

  // factory creates correct agent
  auto &agent = factory.AddAStarAgent(agentData);

  SECTION("Properties") {
    // test properties of created agent
    REQUIRE(agent.GetName() == agentData.name);
    REQUIRE(agent.GetPosition() == agentData.position);
    REQUIRE(agent.GetProperty<char>("symbol") == agentData.symbol);
    REQUIRE(agent.GetGoalPosition() == agentData.goal_pos);
    REQUIRE(agent.GetRecalculateValue() == agentData.recalculate_after_x_turns);
  }

  SECTION("Type") {
    // test type of created agent
    REQUIRE(dynamic_cast<walle::AStarAgent *>(&agent) != nullptr);
  }
}

TEST_CASE("Adding a PacingAgent", "[Agents]") {

  // create a fake world and factory
  MockWorld world;
  AgentFactory factory(world);

  // add properties/data to the factory
  PacingAgentData agentData;
  agentData.name = "PacingAgent";
  agentData.position = {0, 0};
  agentData.symbol = '*';
  agentData.vertical = true;

  auto &agent = factory.AddPacingAgent(agentData);

  SECTION("Properties") {
    // factory creates correct agent
    REQUIRE(agent.GetName() == agentData.name);
    REQUIRE(agent.GetPosition() == agentData.position);
    REQUIRE(agent.GetProperty<char>("symbol") == agentData.symbol);
    REQUIRE(agent.GetVertical() == agentData.vertical);
  }

  SECTION("Type") {
    // test type of created agent
    REQUIRE(dynamic_cast<cse491::PacingAgent *>(&agent) != nullptr);
  }
}

TEST_CASE("Adding a TrackingAgent (no alerter)", "[Agents]") {

  // create a fake world and factory
  MockWorld world;
  cse491::Entity *targ = nullptr;
  AgentFactory factory(world);

  // add properties/data to the factory
  TrackingAgentData agentData;
  agentData.name = "TrackingAgent";
  agentData.position = {0, 0};
  agentData.symbol = '*';
  agentData.string_path = "x";
  agentData.vector_path = StrToOffsets(agentData.string_path);
  agentData.target = targ;
  agentData.tracking_distance = 10;
  agentData.start_pos = {1, 1};

  auto &agent = factory.AddTrackingAgent(agentData);

  SECTION("Properties") {
    // factory creates correct agent
    REQUIRE(agent.GetName() == agentData.name);
    REQUIRE(agent.GetPosition() == agentData.position);
    REQUIRE(agent.GetProperty<char>("symbol") == agentData.symbol);
    REQUIRE(agent.GetTarget() == nullptr);
    REQUIRE(agent.GetAlerter() == nullptr);
  }

  SECTION("Type") {
    // test type of created agent
    REQUIRE(dynamic_cast<TrackingAgent *>(&agent) != nullptr);
  }
}

TEST_CASE("Adding a TrackingAgent (alerter)", "[Agents]") {

  // create a fake world and factory
  MockWorld world;
  cse491::AgentBase a{0, "dummy"};
  cse491::Entity *targ = &a;
  AgentFactory factory(world);

  // add properties/data to the factory
  TrackingAgentData agentData;
  agentData.name = "First";
  agentData.position = {0, 0};
  agentData.symbol = '*';
  agentData.string_path = "x";
  agentData.vector_path = StrToOffsets(agentData.string_path);
  agentData.target = targ;
  agentData.tracking_distance = 10;
  agentData.start_pos = {1, 1};
  agentData.alerter = std::make_shared<Alerter>(&world);

  auto &agent_first = factory.AddTrackingAgent(agentData);

  SECTION("Properties") {
    // factory creates correct agent
    REQUIRE(agent_first.GetName() == agentData.name);
    REQUIRE(agent_first.GetPosition() == agentData.position);
    REQUIRE(agent_first.GetProperty<char>("symbol") == agentData.symbol);
    REQUIRE(agent_first.GetPath() == std::vector<cse491::GridPosition>(1, {0, 0}));
    REQUIRE(agent_first.GetTarget() == &a);
    REQUIRE(agent_first.GetTarget() != nullptr);
  }

  agentData.name = "Second";
  auto &agent_second = factory.AddTrackingAgent(agentData);

  SECTION("Properties [other]") {
    REQUIRE(agent_second.GetTarget() == &a);
    REQUIRE(agent_second.GetTarget() == agent_first.GetTarget());
    REQUIRE(agent_second.GetAlerter() == agent_first.GetAlerter());
  }

  auto alerter = agent_first.GetAlerter();
  SECTION("Alerter properties") {
    auto const& set = alerter->GetNetworkSet();
    REQUIRE(set.contains(agent_first.GetID()));
    REQUIRE(set.contains(agent_second.GetID()));
  }

  SECTION("Alerter properties [removal]") {
    auto const& set = alerter->GetNetworkSet();

    agent_first.RemoveFromAlerter();
    REQUIRE(!set.contains(agent_first.GetID()));
    REQUIRE(agent_first.GetAlerter() == nullptr);

    agent_second.RemoveFromAlerter();
    REQUIRE(!set.contains(agent_second.GetID()));
    REQUIRE(agent_second.GetAlerter() == nullptr);

    REQUIRE(set.empty());
  }
}

TEST_CASE("Adding a PathAgent", "[Agents]") {

  // create a fake world and factory
  MockWorld world;
  AgentFactory factory(world);

  // add properties/data to the factory
  PathAgentData agentData;
  agentData.name = "PathAgent";
  agentData.position = {0, 0};
  agentData.symbol = '*';
  // Path must not be empty
  agentData.string_path = "x";
  // Convert to vector
  agentData.vector_path = StrToOffsets(agentData.string_path);

  auto &agent = factory.AddPathAgent(agentData);

  SECTION("Properties") {
    // factory creates correct agent
    REQUIRE(agent.GetName() == agentData.name);
    REQUIRE(agent.GetPosition() == agentData.position);
    REQUIRE(agent.GetProperty<char>("symbol") == agentData.symbol);
    REQUIRE(agent.GetPath() == agentData.vector_path);
  }

  SECTION("Type") {
    // test type of created agent
    REQUIRE(dynamic_cast<PathAgent *>(&agent) != nullptr);
  }
}