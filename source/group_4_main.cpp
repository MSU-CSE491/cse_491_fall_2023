/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/SecondWorld.hpp"

size_t level_zero = 0;
size_t level_one = 1;
size_t level_two = 2;

int main()
{
  group4::SecondWorld world;
  world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
  world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
  world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');

  // Adding power sword with id = 1; name = sword of power
  auto powerSword = std::make_unique<cse491::ItemBase>(1, "Sword of Power");
  powerSword->SetProperties("symbol", 'S', "Damage", 20.0);
  powerSword->SetPosition(1, 2);
  powerSword->SetGrid(level_zero);
  world.AddItem(std::move(powerSword));

  // Adding fire sword with id = 2; name = Inferno Slicer
  auto infernoSlicer = std::make_unique<cse491::ItemBase>(2, "Inferno Slicer");
  infernoSlicer->SetProperties("symbol", 'S', "Damage", 12.5, "Speed", 15.0, "Burning Duration", 2.5);
  infernoSlicer->SetPosition(3, 4);
  infernoSlicer->SetGrid(level_zero);
  world.AddItem(std::move(infernoSlicer));

  // Adding a piece of armor with id = 10; name = Daedric Armor
  auto daedricArmor = std::make_unique<cse491::ItemBase>(10, "Daedric Armor");
  daedricArmor->SetProperties("symbol", '+', "Health", 99, "Extra Inv. Space", 5);
  daedricArmor->SetPosition(5, 0);
  daedricArmor->SetGrid(level_zero);
  world.AddItem(std::move(daedricArmor));

  // Adding an axe with id = 3; name = Axe of Health
  auto axe = std::make_unique<cse491::ItemBase>(3, "Axe of Health");
  axe->SetProperties("symbol", 'A', "Damage", 8.5, "Health", 35);
  axe->SetPosition(1, 3);
  axe->SetGrid(level_zero);
  world.AddItem(std::move(axe));

  auto dagger = std::make_unique<cse491::ItemBase>(4, "Electric Dagger");
  dagger->SetProperties("symbol", 'D', "Damage", 25);
  dagger->SetPosition(6, 2);
  dagger->SetGrid(level_zero);
  world.AddItem(std::move(dagger));

  auto chest = std::make_unique<cse491::ItemBase>(5, "Ender Chest");
  chest->SetProperties("symbol", 'C', "Chest", 0);
  chest->SetPosition(0, 4);
  chest->SetGrid(level_zero);
  world.AddItem(std::move(chest));

  world.Run();
}
