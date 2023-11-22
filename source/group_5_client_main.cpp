/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A client-side networking main to prototype communication
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.

#include "Interfaces/NetWorth/client/ClientInterface.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Agents/PacingAgent.hpp"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Must have an argument for server IP and port\nUsage: ./client [IP] [port]" << std::endl;
        return 1;
    }

    std::string ip_string(argv[1]);
    unsigned short port = stoi(std::string(argv[2]));

    // will probably need to find a way to receive the world from the server first
    // this will be confusing
    // receive initial world, set of agents, etc?? This will have to be separate from ClientInterface
    cse491::MazeWorld world;
    world.AddAgent<netWorth::ClientInterface>("Interface", "server_ip", ip_string, "server_port", port).SetProperty("symbol", '@');
    //world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    //world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);

    world.Run();
    return 0;
}
