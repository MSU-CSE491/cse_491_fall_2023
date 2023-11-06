/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Agents/AStarAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"

int main() {
  cse491::MazeWorld world;
  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
  auto & astar_agent = 
      static_cast<walle::AStarAgent&>(world.AddAgent<walle::AStarAgent>("AStar 1"));
  astar_agent.SetPosition(7, 3);
  astar_agent.SetGoalPosition(7, 8);
  astar_agent.RecalculatePath();
  world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');

  world.Run();
}
