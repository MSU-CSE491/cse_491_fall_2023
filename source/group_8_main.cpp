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
  
  world.AddItem("Sword", "Strength", 10, "symbol", 't').SetPosition(27, 11);
  world.AddItem("Health Potion", "Healing", 25, "symbol", 'j').SetPosition(38, 16);
  world.AddItem("Health Potion", "Healing", 40, "symbol", 'j').SetPosition(40, 1);

  world.AddAgent<cse491::TrashInterface>("Interface", "symbol", '@').SetPosition(40,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(45, 3);
  world.AddAgent<cse491::PacingAgent>("Pacer 5").SetPosition(3,14);
  world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(7,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 3").SetPosition(18,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 4").SetPosition(45,17);

  world.AddAgent<cse491::PacingAgent>("Shark", "OnlyWater", 1).SetPosition(11, 4);

  auto & pacer_1 = world.GetAgent(world.GetAgentID("Pacer 1"));
  world.AddItem("Sword", "Strength", 15, "symbol", 't').SetPosition(pacer_1.GetPosition());
  world.DoActionAttemptItemPickup(pacer_1, pacer_1.GetPosition());

  auto & interface = world.GetAgent(world.GetAgentID("Interface"));
  world.MoveSetAction(interface);

  world.GenerateMoveSets();
  world.SetBattling();

  world.Run();
  
}
