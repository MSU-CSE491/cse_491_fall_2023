/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Group7_GP_Agent/CGPAgent.hpp"

int main()
{
  cse491::MazeWorld world;
  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
  world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
  world.AddAgent<cowboys::CGPAgent>("GP 1").SetProperty("char", 'G').SetPosition(1, 0);
  world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');

  world.Run();
}
