/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/AgentFactory.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"

void InitializeWorld(cse491::MazeWorld & world, cse491::Entity & player) {
  walle::AgentFactory factory(world);

  auto alerter = std::make_shared<walle::Alerter>(&world);
  walle::TrackingAgentData data_first("Looper", {9, 2}, '$', "e s w n", &player, 4, {9, 2}, alerter);
  factory.AddTrackingAgent(data_first);

  walle::TrackingAgentData data_second("Corner-sitter", {22, 8}, '$', "x", &player, 4, {22, 8}, alerter);
  factory.AddTrackingAgent(data_second);
}

int main() {
  cse491::MazeWorld world;
  auto &player = world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
  InitializeWorld(world, player);
  world.Run();
}
