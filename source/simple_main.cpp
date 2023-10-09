/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Worlds/BiomeGenerator.h"

int main() {
    static const unsigned int SEED = 500;

    BiomeGenerator biomeGenerator("maze", 30, 30, SEED);
    biomeGenerator.generate(' ', '#');
    biomeGenerator.saveToFile("../assets/grids/default_maze.grid");

    BiomeGenerator biomeGenerator2("grasslands", 30, 30, SEED);
    biomeGenerator2.generate('M', '~');
    biomeGenerator2.saveToFile("../assets/grids/default_maze2.grid");

    cse491::MazeWorld world(SEED);
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');

    world.Run();
}
