/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @file AgentFactory.hpp
 * @brief A factory class that abstracts away the initialization of adding an agent to a world
 * @note Status: ALPHA
 * @author Matt Kight
 * @author David Rackerby
 **/

#pragma once

#include "../core/AgentBase.hpp"
#include "../core/Entity.hpp"
#include "../core/WorldBase.hpp"
#include "AStarAgent.hpp"
#include "AgentLibary.hpp"
#include "PacingAgent.hpp"
#include "PathAgent.hpp"
#include "TrackingAgent.hpp"

namespace walle {

// Forward-declare since it's easier to understand how AddXAgent works
// when the structs are defined nearby
struct PacingAgentData;
struct PathAgentData;
struct AStarAgentData;
struct TrackingAgentData;

class AgentFactory {
private:
  cse491::WorldBase &world;  /// The world to create Agents in

public:
  AgentFactory() = delete;

  /**
   * Constructor for AgentFactory
   * @param world we are adding agents too
   */
  explicit AgentFactory(cse491::WorldBase &world) : world(world) {}

  AStarAgent &AddAStarAgent(const AStarAgentData &agent_data);
  cse491::PacingAgent &AddPacingAgent(const PacingAgentData &agent_data);
  TrackingAgent &AddTrackingAgent(const TrackingAgentData &agent_data);
  PathAgent &AddPathAgent(const PathAgentData &agent_data);

};  // class AgentFactory

/**
 * Stores data for AgentBase
 */
struct BaseAgentData {
  /// Name of the agent
  std::string name;

  /// Agent's position
  cse491::GridPosition position;

  /// Agent's representation
  char symbol = '*';
};

/**
 * Stores data for a PacingAgent
 */
struct PacingAgentData : public BaseAgentData {
  /// Whether the PacingAgent is moving up and down (vertical) or left and right(!vertical)
  bool vertical = false;
};

/**
 * Add a PacingAgent to the world
 * @param agent_data data for agent we want to create
 * @return self
 */
cse491::PacingAgent &AgentFactory::AddPacingAgent(const PacingAgentData &agent_data)
{
  auto &entity = world.AddAgent<cse491::PacingAgent>(agent_data.name)
                      .SetPosition(agent_data.position)
                      .SetProperty("symbol", agent_data.symbol);
  auto &agent = DownCastAgent<cse491::PacingAgent>(entity);
  agent.SetVertical(agent_data.vertical);
  return agent;
}

/**
 * Stores data for a PathAgent
 */
struct PathAgentData : public BaseAgentData {
  /// Starting index into the vector of GridPositions
  int index;

  /// String representation of the path traveled (e.g. "n s e w" for north south east west)
  std::string string_path;

  /// Set of grid positions that are applied to the agent's position during one step (constructed
  /// from string_path)
  std::vector<cse491::GridPosition> vector_path;
};

/**
 * Add a PathAgent to the world
 * @param agent_data data for agent we want to create
 * @return self
 */
PathAgent &AgentFactory::AddPathAgent(const PathAgentData &agent_data)
{
  auto &entity = world.AddAgent<walle::PathAgent>(agent_data.name)
                     .SetPosition(agent_data.position)
                     .SetProperty("symbol", agent_data.symbol);
  auto &agent = DownCastAgent<walle::PathAgent>(entity);
  if (!agent_data.string_path.empty()) {
    agent.SetProperty<std::basic_string_view<char>>(
        "path",
        agent_data.string_path);  // TODO add another option to provide grid point
  } else {
    agent.SetPath(agent_data.vector_path);
  }
  agent.Initialize();
  return agent;
}

/**
 * Stores data for an AStarAgent
 */
struct AStarAgentData : public BaseAgentData {
  /// Number of steps after which the shortest path is recalculated
  int recalculate_after_x_turns = 5;

  /// The final position in the world that the AStarAgent is traveling to
  cse491::GridPosition goal_pos;
};

/**
 * Add an AStarAgent to the world
 * @param agent_data data for agent we want to create
 * @return self
 */
AStarAgent &AgentFactory::AddAStarAgent(const AStarAgentData &agent_data)
{
  auto &entity = world.AddAgent<walle::AStarAgent>(agent_data.name)
                     .SetPosition(agent_data.position)
                     .SetProperty("symbol", agent_data.symbol);
  auto &agent = DownCastAgent<walle::AStarAgent>(entity);
  agent.SetGoalPosition(agent_data.goal_pos);
  agent.SetRecalculate(agent_data.recalculate_after_x_turns);
  return agent;
}

/**
 * Stores data for a TrackingAgent
 */
struct TrackingAgentData : public BaseAgentData {
  /// Set of grid positions that are applied to the agent's position during one step (constructed
  /// from string_path) like in PathAgent
  std::vector<cse491::GridPosition> vector_path;

  /// String representation of the path traveled (e.g. "n s e w" for north south east west) like in
  /// PathAgent
  std::string string_path;

  /// Goal Entity being tracked (must not be null or else the agent simply behaves like a PathAgent)
  cse491::Entity *target;

  /// Distance that the TrackingAgent can "see" such that when the target enters that range, it
  /// begins tracking
  int tracking_distance = 5;

  /// Where the TrackingAgent begins from patrolling from and returns two after the target moves out
  /// of range
  cse491::GridPosition start_pos;

  /// Shared reference to an Alerter, which is non-null if the agent should be able to tell other
  /// agents to immediately focus on their targets
  /// @remark You should be using the **same** shared pointer across multiple instances of
  /// TrackingAgentData in order to make the TrackingAgents part of the same network. This means you
  /// need to  copy around this shared pointer when using the factory
  std::shared_ptr<Alerter> alerter = nullptr;

  /// Use initial values
  TrackingAgentData() = default;

  /// Set all values
  TrackingAgentData(std::string name, cse491::GridPosition curr_pos, char symbol, std::string path,
                    cse491::Entity *target, int tracking_dist, cse491::GridPosition start_pos,
                    std::shared_ptr<Alerter> alerter)
      : BaseAgentData({std::move(name), curr_pos, symbol}),
        vector_path(StrToOffsets(path)),
        string_path(std::move(path)),
        target(target),
        tracking_distance(tracking_dist),
        start_pos(start_pos),
        alerter(alerter)
  {
  }
};

/**
 * Add a TrackingAgent to the world
 * @param agent_data data for agent we want to create
 * @return self
 */
TrackingAgent &AgentFactory::AddTrackingAgent(const TrackingAgentData &agent_data)
{
  auto &entity = world.AddAgent<walle::TrackingAgent>(agent_data.name)
                     .SetPosition(agent_data.position)
                     .SetProperty("symbol", agent_data.symbol);
  auto &agent = DownCastAgent<TrackingAgent>(entity);
  if (!agent_data.string_path.empty()) {
    agent.SetProperty<std::basic_string_view<char>>("path", agent_data.string_path);
  } else {
    agent.SetPath(agent_data.vector_path);
  }
  agent.SetTarget(agent_data.target);
  agent.SetTrackingDistance(agent_data.tracking_distance);
  agent.SetStartPosition(agent_data.start_pos);
  if (agent_data.alerter != nullptr) {
    agent.SetProperty("alerter", agent_data.alerter);
  }
  agent.Initialize();
  return agent;
}

}  // namespace walle
