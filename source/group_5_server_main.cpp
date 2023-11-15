/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.

#include <thread>
#include <SFML/Network.hpp>
#include "Agents/PacingAgent.hpp"
#include "Interfaces/NetWorth/server/networkingworld.hpp"
#include "Interfaces/NetWorth/server/ServerInterface.hpp"
#include "Interfaces/NetWorth/server/ServerManager.hpp"


void ClientThread(){

}

/**
 * The initial connection for the server to a client
 * @param sender address of the sender
 * @param port port of the connection
 * @return true if successful
 */
void HandleConnection(netWorth::ServerManager &serverManager, netWorth::NetworkMazeWorld &world) {
    sf::UdpSocket socket;
    if (socket.bind(netWorth::ServerManager::m_initConnectionPort) != sf::Socket::Status::Done){
        std::cerr << "Failed to bind" << std::endl;
        exit(0);
    }

    sf::Packet pkt;
    std::string str;

    std::optional<sf::IpAddress> sender;
    unsigned short port;

    if (socket.receive(pkt, sender, port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to receive" << std::endl;
        exit(0);
    }

    std::cout << "Connection received from IP Address: " << sender->toString() << " on port: " << port << std::endl;

    serverManager.IncreasePort();
    world.AddAgent<netWorth::ServerInterface>("Interface", "client_ip", sender->toString(),
                                              "client_port", port, "server_port", serverManager.m_maxClientPort)
                                              .SetProperty("symbol", '@');

    std::thread clientThread(ClientThread);
    serverManager.AddClient(clientThread);
    std::cout << "Added thread" << std::endl;
}

int main() {

    std::cout << "Server started." << std::endl;
    netWorth::NetworkMazeWorld world;

    netWorth::ServerManager serverManager;
    //world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    //world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);

    std::thread connectionThread(HandleConnection, std::ref(serverManager), std::ref(world));
    std::cout << "Handling connection." << std::endl;

    // will probably need to override world Run function for multiple clients
    // assuming we use NetworkMazeWorld rather than MazeWorld
    // that could be difficult for multiple world classes though...

    world.Run();
    serverManager.JoinClients();
    connectionThread.join();
    return 0;
}

