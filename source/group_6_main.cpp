/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Interfaces/MainInterface.hpp"
#include "Worlds/GenerativeWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"

using namespace group6;

int main() {
//    static const unsigned int SEED = time(nullptr);
    static const unsigned int SEED = 5;
    auto biome = BiomeType::Maze; // specify biome type here

    GenerativeWorld world(biome, 100, 20, SEED);
    world.AddItem("Boots", "symbol", 'B').SetPosition(1, 3).SetName("Boots").SetProperty("Health", 4.0);
    world.AddItem("Shield", "symbol", 'S').SetPosition(1, 4).SetName("Shield").SetProperty("Health", 4.0);
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
//    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');
    world.AddAgent<i_2D::MainInterface>("Interface2").SetProperty("symbol", '@');

    world.AddTeleporters();

    world.AddArmory();
    world.Run();
}