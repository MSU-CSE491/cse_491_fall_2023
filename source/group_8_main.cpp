/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 * @author Grant Carr
 * @author Evan Orvis
 * @author Nate Gu
 * @author Lauren Garvey
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/MainInterface.hpp"
#include "Worlds/ManualWorld.hpp"
#include "Agents/AStarAgent.hpp"

int main()
{
  cse491_team8::ManualWorld world;

  world.AddItem("Axe", "Uses", 5, "symbol", 'P').SetPosition(80, 120);
  world.AddItem("Axe", "Uses", 10, "symbol", 'P').SetPosition(97, 40);

  world.AddItem("Boat", "Uses", 7, "symbol", 'U').SetPosition(55, 11);

  world.AddItem("Sword", "Strength", 8, "symbol", 't').SetPosition(18, 4);
  world.AddItem("Sword", "Strength", 5, "symbol", 't').SetPosition(27, 11);
  world.AddItem("Sword", "Strength", 4, "symbol", 't').SetPosition(65, 89);

  world.AddItem("Health Potion", "Healing", 25, "symbol", 'j').SetPosition(38, 16);
  world.AddItem("Health Potion", "Healing", 30, "symbol", 'j').SetPosition(1, 18);

  cse491::AgentBase& player = 
    world.AddAgent<i_2D::MainInterface>("Interface3", "symbol", '@');
  player.SetPosition(80,63);
  dynamic_cast<i_2D::MainInterface&>(player).SetLargeGrid(true);
  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(97, 45);
  world.AddAgent<cse491::PacingAgent>("Pacer 5").SetPosition(3,14);
  world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(7,30);
  world.AddAgent<cse491::PacingAgent>("Pacer 6").SetPosition(27, 10);
  world.AddAgent<cse491::PacingAgent>("Pacer 7").SetPosition(38, 10);
  world.AddAgent<cse491::PacingAgent>("Pacer 3").SetPosition(18,3);
  world.AddAgent<cse491::PacingAgent>("Pacer 4").SetPosition(45,17);

  auto & a_star_agent = static_cast<walle::AStarAgent&>(world.AddAgent<walle::AStarAgent>("AStar 1"));
  a_star_agent.SetPosition(80, 111);
  a_star_agent.SetGoalPosition(80, 63);

  world.AddAgent<cse491::PacingAgent>("Shark", "OnlyWater", 1).SetPosition(125, 140);

  auto & pacer_1 = world.GetAgent(world.GetAgentID("Pacer 1"));
  world.AddItem("Sword", "Strength", 15, "symbol", 't').SetPosition(pacer_1.GetPosition());
  world.DoActionAttemptItemPickup(pacer_1, pacer_1.GetPosition());

  auto & interface = world.GetAgent(world.GetAgentID("Interface3"));
  world.MoveSetAction(interface);

  world.GenerateMoveSets();

  world.Run();
  
}
