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
  auto &player = world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');

  auto alerter = std::make_shared<walle::Alerter>(&world);

  auto &looper_base = world.AddAgent<walle::TrackingAgent>("Looper").SetPosition(9, 2).SetProperty("symbol", '$');
  assert(dynamic_cast<walle::TrackingAgent *>(&looper_base));
  auto& looper = static_cast<walle::TrackingAgent &>(looper_base);
  looper.SetProperty<std::basic_string_view<char>>("path", "e s w n");
  looper.SetProperty("alerter", alerter);
  looper.SetTarget(&player);
  looper.SetTrackingDistance(4);
  looper.Initialize();

  auto &corner_base = world.AddAgent<walle::TrackingAgent>("Corner-sitter").SetPosition(22, 8).SetProperty("symbol", '$');
  assert(dynamic_cast<walle::TrackingAgent *>(&corner_base));
  auto& corner = static_cast<walle::TrackingAgent &>(corner_base);
  corner.SetProperty<std::basic_string_view<char>>("path", "x");
  corner.SetProperty("alerter", alerter);
  corner.SetTarget(&player);
  corner.SetTrackingDistance(4);
  corner.Initialize();

  world.Run();
}
