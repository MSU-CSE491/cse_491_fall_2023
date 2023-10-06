/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/ManualWorld.hpp"

int main()
{
  cse491_team8::ManualWorld world;
  world.AddAgent<cse491::TrashInterface>("Interface", "Strength", 7, "Health", 20).SetProperty("char", '@').SetPosition(40,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 5, "Health", 5).SetPosition(45, 3);
  world.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 2, "Health", 10).SetPosition(3,14);
  world.AddAgent<cse491::PacingAgent>("Pacer 2", "Strength", 4, "Health", 20).SetPosition(7,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 3", "Strength", 6, "Health", 30).SetPosition(18,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 4", "Strength", 8, "Health", 40).SetPosition(45,17);

  world.Run();
}
