//
// Created by Matthew Kight on 9/24/23.
//

#pragma once

#include "AStarAgent.hpp"
#include "PacingAgent.hpp"
#include "PathAgent.hpp"
#include "TrackingAgent.hpp"

#include "../core/Entity.hpp"
#include "../core/WorldBase.hpp"
namespace walle {

struct BaseAgentData {
    std::string name;
    cse491::GridPosition position;
    char symbol = '*';
};

struct AStarAgentData : public BaseAgentData {
    int recalculate_after_x_turns = 5;
    cse491::GridPosition target;
};

struct PacingAgentData : public BaseAgentData {
    bool vertical = false;
};

struct TrackingAgentData : public BaseAgentData {
    std::string path;
    std::vector<cse491::GridPosition> vector_path;
    cse491::Entity *target;
    int tracking_distance = 5;
    cse491::GridPosition start_pos;
};

struct PathAgentData : public BaseAgentData {
    std::string path;
    std::vector<cse491::GridPosition> vector_path;
};

class AgentFactory {
 private:
    cse491::WorldBase &world;
 public:
    AgentFactory() = delete;
    /**
     * Constructor for AgentFactory
     * @param world we are adding agents too
     */
    AgentFactory(cse491::WorldBase &world) : world(world) {}

    /**
     * Add Agent to the world
     * @param agent_data data for agent we want to create
     * @return self
     */
    AStarAgent &AddAStarAgent(const AStarAgentData &agent_data) {
        auto &entity = world.AddAgent<walle::AStarAgent>(agent_data.name).SetPosition(agent_data.position).SetProperty(
            "symbol",
            agent_data.symbol);
        assert(dynamic_cast<walle::AStarAgent *>(&entity));
        auto &agent = static_cast<walle::AStarAgent &>(entity);
        agent.SetGoalPosition(agent_data.target);
        agent.SetRecalculate(agent_data.recalculate_after_x_turns);

        return agent;
    }
    /**
     * Add Agent to the world
     * @param agent_data data for agent we want to create
     * @return self
     */
    cse491::PacingAgent &AddPacingAgent(const PacingAgentData &agent_data) {
        auto
            &entity = world.AddAgent<cse491::PacingAgent>(agent_data.name).SetPosition(agent_data.position).SetProperty(
            "symbol",
            agent_data.symbol);
        assert(dynamic_cast<cse491::PacingAgent *>(&entity));
        auto &agent = static_cast<cse491::PacingAgent &>(entity);
        agent.SetVertical(agent_data.vertical);
        return agent;
    }
    /**
   * Add Agent to the world
   * @param agent_data data for agent we want to create
   * @return self
   */
    TrackingAgent &AddTrackingAgent(const TrackingAgentData &agent_data) {
        auto &entity =
            world.AddAgent<walle::TrackingAgent>(agent_data.name).SetPosition(agent_data.position).SetProperty(
                "symbol",
                agent_data.symbol);
        assert(dynamic_cast<walle::TrackingAgent *>(&entity));
        auto &agent = static_cast<walle::TrackingAgent &>(entity);
        if (!agent_data.path.empty())
            agent.SetProperty<std::basic_string_view<char>>("path",
                                                            agent_data.path);
        else
            agent.SetPath(agent_data.vector_path);
        agent.SetTarget(agent_data.target);
        agent.SetTrackingDistance(agent_data.tracking_distance);
        agent.SetStartPosition(agent_data.start_pos);
        agent.Initialize();
        return agent;
    }

    /**
   * Add Agent to the world
   * @param agent_data data for agent we want to create
   * @return self
   */
    PathAgent &AddPathAgent(const PathAgentData &agent_data) {
        auto &entity =
            world.AddAgent<walle::PathAgent>(agent_data.name).SetPosition(agent_data.position).SetProperty(
                "symbol",
                agent_data.symbol);
        assert(dynamic_cast<walle::PathAgent *>(&entity));
        auto &agent = static_cast<walle::PathAgent &>(entity);
        if (!agent_data.path.empty())
            agent.SetProperty<std::basic_string_view<char>>("path",
                                                            agent_data.path); // TODO add another option to provide grid point
        else
            agent.SetPath(agent_data.vector_path);
        agent.Initialize();
        return agent;
    }
};
}

