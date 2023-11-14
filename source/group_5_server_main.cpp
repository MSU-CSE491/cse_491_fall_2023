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

    std::optional<sf::IpAddress> sender;  /// the destination IP of the machine this communicates with
    unsigned short port;          /// the destination port of the machine this communicates with

    if (socket.receive(pkt, sender, port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to receive" << std::endl;
        exit(0);
    }

    std::cout << "Connection received from IP Address: " << sender.value() << " on port: " << port << std::endl;

    world.AddAgent<netWorth::ServerInterface>("Interface").SetProperty("symbol", '@');

    serverManager.IncreasePort();

    sf::UdpSocket clientSocket;
    if (clientSocket.bind(serverManager.m_maxClientPort) != sf::Socket::Status::Done){
        std::cerr << "Failed to bind" << std::endl;
        exit(0);
    }

    std::thread clientThread(ClientThread);
    serverManager.AddClient(clientThread);
//    return true;

//
//    std::cout << sf::IpAddress::getLocalAddress().value() << std::endl;
//    BindSocket(m_socket, m_initConnectionPort);
//
//    // Await client
//    if (!NetworkingInterface::ReceivePacket(recv_pkt, sender, port)) return false;
//    RedirectClient();
//
//    recv_pkt >> str;
//    std::cout << str << std::endl;
//    std::cout << sender.value() << " has connected successfully." << std::endl;
//
//    // Acknowledge client
//    std::string clientAck = "Connection established on server port: " + std::to_string(m_maxClientPort);
//    send_pkt << clientAck;
//    if (!SendPacket(send_pkt, sender.value(), port)) return false;
//
//    // await request for map
//    if (!ReceivePacket(recv_pkt, sender, port)) return false;
//
//    recv_pkt >> str;
//    std::cout << str << std::endl;
//
//    return true;
}

int main() {

    std::cout << "Server started." << std::endl;
    netWorth::NetworkMazeWorld world;

    netWorth::ServerManager serverManager;
    //world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    //world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);

    std::cout << "Handling connection." << std::endl;
    std::thread connectionThread(HandleConnection, std::ref(serverManager), std::ref(world));

    // will probably need to override world Run function for multiple clients
    // assuming we use NetworkMazeWorld rather than MazeWorld
    // that could be difficult for multiple world classes though...

    world.Run();
    serverManager.JoinClients();
    connectionThread.join();
    return 0;
}

