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
    static const unsigned int SEED = 933;

    BiomeGenerator biomeGenerator(BiomeType::Maze, 100, 20, SEED);
    biomeGenerator.generate();
//    if (biomeGenerator.isPathExists()) {
//        std::cout << "eeee" << std::endl;
//    } else {
//        std::cout << "iiii" << std::endl;
//        // No path exists.
//    }
    biomeGenerator.saveToFile("../assets/grids/default_maze.grid");

    BiomeGenerator biomeGenerator2(BiomeType::Grasslands, 30, 30, SEED);
    biomeGenerator2.generate();
    biomeGenerator2.saveToFile("../assets/grids/default_maze2.grid");

    cse491::MazeWorld world(SEED);
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');

    world.Run();
}
