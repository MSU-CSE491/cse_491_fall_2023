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


void ClientThread(netWorth::ServerInterface & interface, netWorth::NetworkMazeWorld &world,
                  netWorth::ServerManager & serverManager){
    //While this client is still connected (need to fix)
    while (serverManager.m_action_map.contains(interface.GetSocket()->getLocalPort())){
        sf::Packet clientPacket;
        std::optional<sf::IpAddress> clientIP;
        unsigned short clientPort;

//        size_t clientAction = interface.SelectAction(world.GetGrid(), world.GetCellTypes(), world.Get);

        //Might need to do this inside of do action or before DoAction.
//        serverManager.m_action_map.insert_or_assign(interface.GetSocket()->getLocalPort(), clientAction);
    }
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
    //networth::ServerInterface & interface


    cse491::Entity & interface = world.AddAgent<netWorth::ServerInterface>("Interface", "client_ip", sender->toString(),
                                              "client_port", port, "server_port", serverManager.m_maxClientPort)
                                              .SetProperty("symbol", '@');

    auto & serverInterface = dynamic_cast<netWorth::ServerInterface &>(interface);


    std::thread clientThread(ClientThread, std::ref(serverInterface), std::ref(world),
                             std::ref(serverManager));
    serverManager.AddClient(clientThread, serverManager.m_maxClientPort);
    std::cout << "Added thread" << std::endl;

}

int main() {

    std::cout << "Server started on IP address: " << sf::IpAddress::getLocalAddress()->toString() << std::endl;
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
    //serverManager.JoinClients();
    //connectionThread.join();
    return 0;
}

