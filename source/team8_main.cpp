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
  world.AddEntity("Axe", "Chop", 5, "symbol", 'P').SetPosition(37, 3);
  world.AddEntity("Boat", "Swim", 7, "symbol", 'U').SetPosition(18, 4);
  world.AddAgent<cse491::TrashInterface>("Interface", "Strength", 7, "Health", 20, "Max_Health", 40, "Direction", 1).SetProperty("symbol", '@').SetPosition(40,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 5, "Health",  5, "Max_Health", 30, "Direction", 1).SetPosition(45, 3);
  world.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 2, "Health", 10, "Max_Health", 30, "Direction", 1).SetPosition(3,14);
  world.AddAgent<cse491::PacingAgent>("Pacer 2", "Strength", 4, "Health", 20, "Max_Health", 30, "Direction", 1).SetPosition(7,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 3", "Strength", 6, "Health", 30, "Max_Health", 30, "Direction", 1).SetPosition(18,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 4", "Strength", 8, "Health", 40, "Max_Health", 30, "Direction", 1).SetPosition(45,17);

  world.Run();
}
