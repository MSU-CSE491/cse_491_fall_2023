/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Worlds/NewWorld.hpp"

int main()
{
  cse491_team8::NewWorld world;
  world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@').SetProperty("Strength", 7);;
  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1).SetProperty("Strength", 2);
  world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1).SetProperty("Strength", 4);;
  world.AddAgent<cse491::PacingAgent>("Pacer 3").SetPosition(18,3).SetProperty("Strength", 6);;
  world.AddAgent<cse491::PacingAgent>("Pacer 4").SetPosition(21,3).SetProperty("Strength", 8);;

  world.Run();
}
