/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Group 6 main file
 **/

// Include the modules that we will be using.
#include <ctime>
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"

using group6::BiomeGenerator, group6::BiomeType;

int main() {
    static const unsigned int SEED = 973;

    BiomeGenerator biomeGenerator(BiomeType::Maze, 110, 25, SEED);
    biomeGenerator.generate();

    srand(time(nullptr));
    auto path = biomeGenerator.clearPath();
    biomeGenerator.applyPathToGrid(path);
    biomeGenerator.saveToFile("../assets/grids/default_maze.grid");

    cse491::MazeWorld world(SEED);
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');

    world.Run();
}