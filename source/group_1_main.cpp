/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/AStarAgent.hpp"
#include "Agents/RandomAgent.hpp"
#include "Agents/TrackingAgent.hpp"

#include "Agents/AgentFactory.hpp"
#include "Agents/PacingAgent.hpp"

#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"

int main() {
    cse491::MazeWorld world;
    auto &player = world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
    walle::AgentFactory factory(world);
    /*
    walle::TrackingAgentData agent_data;
    agent_data.name = "looper";
    agent_data.position = cse491::GridPosition(9, 2);
    agent_data.target = &player;
    agent_data.tracking_distance = 4;
    agent_data.start_pos = cse491::GridPosition(9, 2);
    agent_data.path = "e s w n";
    factory.AddTrackingAgent(agent_data);
    */
    /*
    walle::AStarAgentData agent_data;
    agent_data.name = "AStar";
    agent_data.symbol = 'X';
    agent_data.position = cse491::GridPosition(9, 2);
    agent_data.target = cse491::GridPosition(0, 1);
    factory.AddAStarAgent(agent_data);
     */
    /*
    walle::PacingAgentData agent_data;
    agent_data.name = "Pacing";
    agent_data.position = cse491::GridPosition(9, 2);
    agent_data.vertical = false;
    factory.AddPacingAgent(agent_data);
     */
    /*
    walle::PathAgentData agent_data;
    agent_data.name = "path";
    agent_data.position = cse491::GridPosition(9, 2);
    agent_data.path = "e s w n";
    factory.AddPathAgent(agent_data);
     */

    world.Run();
}
