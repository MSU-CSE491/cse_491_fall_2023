/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.

#include <thread>
#include <mutex>
#include <SFML/Network.hpp>
#include "Agents/PacingAgent.hpp"
#include "Interfaces/NetWorth/server/networkingworld.hpp"
#include "Interfaces/NetWorth/server/ServerInterface.hpp"
#include "Interfaces/NetWorth/server/ServerManager.hpp"


void ClientThread(netWorth::ServerInterface & interface, netWorth::NetworkMazeWorld &world,
                  netWorth::ServerManager & serverManager){
    // Send to acknowledge client

    std::cout << "In client thread" << std::endl;

    std::cout << interface.GetName() << std::endl;

    //While this client is still connected (need to fix)
    while (serverManager.ActionMapContains(interface.GetID())){
        std::cout << interface.GetName() << " is connected" << std::endl;
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
    pkt >> str;
    std::cout << str << std::endl;


    serverManager.IncreasePort();

    pkt.clear();
    pkt << serverManager.m_maxClientPort;
    if (socket.send(pkt, sender.value(), port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send" << std::endl;
        exit(0);
    }

    std::string serverInterfaceName = "ServerInterface" + std::to_string(serverManager.m_maxClientPort);

    cse491::Entity & interface = world.AddAgent<netWorth::ServerInterface>(serverInterfaceName, "client_ip", sender->toString(),
                                              "client_port", port, "server_port", serverManager.m_maxClientPort)
                                              .SetProperty("symbol", '@');

    auto & serverInterface = dynamic_cast<netWorth::ServerInterface &>(interface);

    //Do an atomic check to see if you can add it
    serverManager.WriteToActionMap(serverInterface.GetID(), 0);

    std::thread clientThread(ClientThread, std::ref(serverInterface), std::ref(world),
                             std::ref(serverManager));

    serverManager.AddToThreadVector(clientThread);
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

