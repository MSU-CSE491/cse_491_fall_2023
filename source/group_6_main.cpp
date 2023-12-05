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
#include "Agents/AStarAgent.hpp"
#include <chrono>
#include <thread>


using namespace group6;

void timer(GenerativeWorld& world, int duration) {
    std::this_thread::sleep_for(std::chrono::seconds(duration));
    world.EndGame(false);
}


int main() {
    static const unsigned int SEED = 16;   // change world seed here
    auto biome = BiomeType::Maze;          // change world biome type here
    int width = 100;                       // change world width here
    int height = 25;                       // change world height here
    int timerDuration = 5;                 // an optional timer length

    GenerativeWorld world(biome, width, height, SEED);  // create our world

    // Add in our items
    world.AddItem("Boots", "symbol", 'B').SetPosition(1, 3).SetName("Boots").SetProperty("Health", 4.0);
    world.AddItem("Shield", "symbol", 'S').SetPosition(1, 4).SetName("Shield").SetProperty("Health", 4.0);

    // Add in our agents: Pacing and A*
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);
    auto & astar_agent = static_cast<walle::AStarAgent&>(world.AddAgent<walle::AStarAgent>("AStar1"));
    astar_agent.SetPosition(17, 17);
    astar_agent.SetGoalPosition(1, 1);

//    astar_agent.RecalculatePath();
//    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", '@');

    // Main interface for the map (use with Maze only, not Ocean or Grasslands: Use Trash Interface for those)
    world.AddAgent<i_2D::MainInterface>("Interface2").SetProperty("symbol", '@').SetName("Player");

    // Add Teleporters and Armory
    world.AddTeleporters();
    world.AddTeleporters();
    world.AddArmory();



    // Start an optional timer
    std::thread timerThread(timer, std::ref(world), timerDuration);
    world.Run();
    timerThread.join();

}