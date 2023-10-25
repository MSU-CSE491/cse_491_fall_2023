/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/MainInterface.hpp"
#include "Worlds/SecondWorld.hpp"

int main()
{
  group4::SecondWorld world;
//  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
//  world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
  //world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
  world.AddAgent<i_2D::MainInterface>("Interface").SetProperty<char>("symbol", '@');

  // Adding power sword with id = 1; name = sword of power
  auto powerSword = std::make_unique<cse491::ItemBase>(1, "Sword of Power");
  powerSword->SetProperty("Damage", 20.0);
  powerSword->SetPosition(1, 2);
  world.AddItem(std::move(powerSword));

  // Adding fire sword with id = 2; name = Inferno Slicer
  auto infernoSlicer = std::make_unique<cse491::ItemBase>(2, "Inferno Slicer");
  infernoSlicer->SetProperties("Damage", 12.5, "Speed", 15.0, "Burning Duration", 2.5);
  infernoSlicer->SetPosition(3, 4);
  world.AddItem(std::move(infernoSlicer));

  // Adding a piece of armor with id = 10; name = Daedric
  auto daedricArmor = std::make_unique<cse491::ItemBase>(10, "Daedric Armor");
  daedricArmor->SetProperties("Health", 99, "Extra Inv. Space", 5);
  daedricArmor->SetPosition(5, 0);
  world.AddItem(std::move(daedricArmor));

  world.Run();
}
