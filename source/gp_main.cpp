/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "Agents/PacingAgent.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Worlds/MazeWorld.hpp"

#include "Group7_GP_Agent/agent.hpp"
#include "Group7_GP_Agent/LGPAgent.hpp"


int main() {
    cse491::MazeWorld world;
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);

    //GP agent
    world.AddAgent<cowboys::LGPAgent>("GP 1").SetPosition(1, 0).SetProperty("char", 'G');


    // Human agent
    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');


    world.Run();
}
