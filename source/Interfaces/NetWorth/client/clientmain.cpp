/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A client-side networking main to prototype communication
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "ClientInterface.hpp"
#include "../server/networkingworld.hpp"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Must have an argument for server IP and port\nUsage: ./client [IP] [port]" << std::endl;
        return 1;
    }

    std::string ip_string(argv[1]);
    unsigned short port = stoi(std::string(argv[2]));

    // will probably need to find a way to receive the world from the server first
    // this will be confusing
    netWorth::NetworkMazeWorld world;
    world.AddAgent<netWorth::ClientInterface>("Interface", "ip", ip_string, "port", port).SetProperty("symbol", '@');

    // will probably need to override world Run function for multiple clients
    world.Run();
    return 0;
}
