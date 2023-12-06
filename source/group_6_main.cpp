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

//void timer(GenerativeWorld& world, int duration) {
//    std::this_thread::sleep_for(std::chrono::seconds(duration));
//    world.EndGame(false);
//}

int main() {
    auto biome = BiomeType::Grasslands;          // change world biome type here
    int width = 40;                       // change world width here
    int height = 20;                       // change world height here
    static const unsigned int SEED = 27;   // change world seed here

    GenerativeWorld world(biome, width, height, SEED);

//    // Start an optional timer
//    if (timerDuration != -1) {
//        std::thread timerThread(timer, std::ref(world), timerDuration);
//        world.Run();
//        timerThread.join();
//    }

    world.AddAgent<i_2D::MainInterface>("Interface2").SetProperty("symbol", '@').SetName("Player");

    world.Run();

    return 0;
}