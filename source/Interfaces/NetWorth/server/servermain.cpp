/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "../../../Agents/PacingAgent.hpp"
#include "networkingworld.hpp"
#include "ServerInterface.hpp"

int main() {
    netWorth::NetworkMazeWorld world;
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
    world.AddAgent<netWorth::ServerInterface>("Interface").SetProperty("symbol", '@');

    // will probably need to override world Run function for multiple clients
    world.Run();
    return 0;
}