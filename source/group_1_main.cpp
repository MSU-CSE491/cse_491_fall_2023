/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/AStarAgent.hpp"
#include "Agents/TrackingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"

int main() {
  cse491::MazeWorld world;
  auto &entity = world.AddAgent<walle::TrackingAgent>("Looper").SetPosition(9, 2).SetProperty("symbol", '$');
  assert(dynamic_cast<walle::TrackingAgent *>(&entity));
  auto& looper = static_cast<walle::TrackingAgent &>(entity);
  looper.SetProperty<std::basic_string_view<char>>("path", "e s w n");
  looper.Initialize();
  auto &player = world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
  looper.SetTarget(&player);
  looper.SetTrackingDistance(1000);
  world.Run();
}
