/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/AStarAgent.hpp"
#include "Agents/RandomAgent.hpp"
#include "Agents/TrackingAgent.hpp"
#include "Agents/PacingAgent.hpp"
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

  auto & random_agent =
		static_cast<walle::RandomAgent&>(world.AddAgent<walle::RandomAgent>("Random 1"));
  random_agent.SetPosition(3, 6);

  world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
  auto &entity = world.AddAgent<walle::TrackingAgent>("Looper").SetPosition(9, 2).SetProperty("symbol", '$');
  assert(dynamic_cast<walle::TrackingAgent *>(&entity));
  auto& looper = static_cast<walle::TrackingAgent &>(entity);
  looper.SetProperty<std::basic_string_view<char>>("path", "e s w n");
  looper.Initialize();
  auto &player = world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
  looper.SetTarget(&player);
  world.Run();
}
