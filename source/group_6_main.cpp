/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/GenerativeWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"

int main() {
    static const unsigned int SEED = 973;
    BiomeGenerator biomeGenerator(BiomeType::Maze, 110, 25, SEED);
    biomeGenerator.generate();

    srand(time(NULL));
    auto path = biomeGenerator.clearPath();
    biomeGenerator.applyPathToGrid(path);
    biomeGenerator.saveToFile("../assets/grids/generated_maze.grid");

    cse491::GenerativeWorld world(SEED);
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');

    world.Run();
}
