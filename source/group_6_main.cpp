/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include <ctime>
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/GenerativeWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"

using namespace group6;

int main() {
    static const unsigned int SEED = 5;
    auto biome = BiomeType::Maze; // specify biome type here
    BiomeGenerator biomeGenerator(biome, 100, 20, SEED);
    biomeGenerator.generate();

    srand(time(nullptr));

    if (biome == BiomeType::Maze)
    {
        auto path = biomeGenerator.clearPath();
        biomeGenerator.applyPathToGrid(path);
    }

    biomeGenerator.saveToFile("../assets/grids/generated_maze.grid");

    cse491::GenerativeWorld world(SEED);
    world.AddItem("Boots", "symbol", 'B').SetPosition(1, 3).SetName("Boots");
    world.AddItem("Boots", "symbol", 'S').SetPosition(1, 4).SetName("Shield");
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');

    world.Run();
}