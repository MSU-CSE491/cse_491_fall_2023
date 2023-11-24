/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/MainInterface.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Worlds/SecondWorld.hpp"
#include "Worlds/ManualWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"
#include "Worlds/GenerativeWorld.hpp"

int main() {
//    cse491::MazeWorld world;
//    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
//    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
//    world.AddAgent<i_2D::MainInterface>("Interface1").SetProperty<char>("symbol", '@');
//    world.Run();
//
//     group 4 maze -1
    group4::SecondWorld world_1;

    world_1.AddAgent<i_2D::MainInterface>("Interface").SetProperty<char>("symbol", '@');

    // Adding power sword with id = 1; name = sword of power
    auto powerSword = std::make_unique<cse491::ItemBase>(1, "Sword of Power");
    powerSword->SetProperty("Damage", 20.0);
    powerSword->SetPosition(1, 2);
    world_1.AddItem(std::move(powerSword));

    // Adding fire sword with id = 2; name = Inferno Slicer
    auto infernoSlicer = std::make_unique<cse491::ItemBase>(2, "Inferno Slicer");
    infernoSlicer->SetProperties("Damage", 12.5, "Speed", 15.0, "Burning Duration", 2.5);
    infernoSlicer->SetPosition(3, 4);


    // Adding a piece of armor with id = 10; name = Daedric
    auto daedricArmor = std::make_unique<cse491::ItemBase>(10, "Daedric Armor");
    daedricArmor->SetProperties("Health", 99, "Extra Inv. Space", 5);
    daedricArmor->SetPosition(5, 0);
    world_1.AddItem(std::move(daedricArmor));
    world_1.Run();

    static const unsigned int SEED = 973;
    BiomeGenerator biomeGenerator(BiomeType::Maze, 110, 25, SEED);
    biomeGenerator.generate();

    srand(time(NULL));
    auto path = biomeGenerator.clearPath();
    biomeGenerator.applyPathToGrid(path);
    biomeGenerator.saveToFile("../assets/grids/generated_maze.grid");

    cse491::GenerativeWorld world_2(SEED);
    world_2.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    world_2.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
    world_2.AddAgent<i_2D::MainInterface>("Interface2").SetProperty("symbol", '@');

    world_2.Run();

    cse491_team8::ManualWorld world_3;
    world_3.AddItem("Axe", "Chop", 5, "symbol", 'P').SetPosition(37, 3);
    world_3.AddItem("Boat", "Swim", 7, "symbol", 'U').SetPosition(18, 4);
    world_3.AddAgent<i_2D::MainInterface>("Interface3", "Strength", 15, "Health", 15, "Max_Health", 40, "Direction",
                                          0).SetProperty("symbol", '@').SetPosition(40, 3);
    world_3.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 30, "Health", 5, "Max_Health", 30, "Direction",
                                          0).SetPosition(45, 3);
    world_3.AddAgent<cse491::PacingAgent>("Pacer 1", "Strength", 15, "Health", 10, "Max_Health", 30, "Direction",
                                          0).SetPosition(3, 14);
    world_3.AddAgent<cse491::PacingAgent>("Pacer 2", "Strength", 20, "Health", 20, "Max_Health", 30, "Direction",
                                          0).SetPosition(7, 3);
    world_3.AddAgent<cse491::PacingAgent>("Pacer 3", "Strength", 25, "Health", 30, "Max_Health", 30, "Direction",
                                          0).SetPosition(18, 3);
    world_3.AddAgent<cse491::PacingAgent>("Pacer 4", "Strength", 30, "Health", 40, "Max_Health", 30, "Direction",
                                          0).SetPosition(45, 17);

    world_3.GenerateMoveSets();


    world_3.Run();


}
