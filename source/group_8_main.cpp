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

  world.AddItem("Axe", "Uses", 5, "symbol", 'P').SetPosition(37, 3);
  world.AddItem("Axe", "Uses", 10, "symbol", 'P').SetPosition(40, 5);
  world.AddItem("Boat", "Uses", 7, "symbol", 'U').SetPosition(18, 4);
  
  world.AddItem("Sword", "Strength", 4, "symbol", 't').SetPosition(27, 11);
  world.AddItem("Health Potion", "Health", 6, "symbol", 'j').SetPosition(38, 16);

  world.AddAgent<cse491::TrashInterface>("Interface", "Strength", 15, "Health", 15, "Max_Health", 40, "Direction", 0).SetProperty("symbol", '@').SetPosition(40,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 30, "Health",  5, "Max_Health", 30, "Direction", 0).SetPosition(45, 3);
  world.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 15, "Health", 10, "Max_Health", 30, "Direction", 0).SetPosition(3,14);
  world.AddAgent<cse491::PacingAgent>("Pacer 2", "Strength", 20, "Health", 20, "Max_Health", 30, "Direction", 0).SetPosition(7,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 3", "Strength", 25, "Health", 30, "Max_Health", 30, "Direction", 0).SetPosition(18,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 4", "Strength", 30, "Health", 40, "Max_Health", 30, "Direction", 0).SetPosition(45,17);

  world.AddAgent<cse491::PacingAgent>("Shark", "Strength", 30, "Health", 40, "Max_Health", 30, "Direction", 0, "OnlyWater", 1).SetPosition(11, 4);

  world.GenerateMoveSets();
  
  world.Run();
  
}
