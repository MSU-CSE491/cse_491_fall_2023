/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"

int main() {
  DataCollection::AgentReceiver agent_receiver;
  cse491::MazeWorld world;
  world.SetAgentReceiver(agent_receiver);
  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
  world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
  world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');

  world.Run();
}
