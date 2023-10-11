/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Interfaces/MainInterface.h"
#include "Worlds/MazeWorld.hpp"

#include "Interfaces/Menu.h"

int main()
{
//    i_2D::Menu menu(600.f,600.f);
//    menu.Start();
  cse491::MazeWorld world;
  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
  world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
//  world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');
  world.AddAgent<i_2D::MainInterface>("Interface").SetProperty("char", '@');

  world.Run();
}
