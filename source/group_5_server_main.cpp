/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include <sstream>
#include <string>
#include <vector>
#include "core/Data.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Agents/PacingAgent.hpp"
#include "Worlds/MazeWorld.hpp"
//#include "../NetworkInterface.hpp"
#include "Interfaces/NetWorth/server/networkingworld.hpp"
#include "Interfaces/NetWorth/server/ServerPlayerInterface.hpp"

int main()
{
    //Create the world on the server side upon initialization
    //Add the pacing agents to just simply walk around
    //Once everything is added we simply wait for a response from a client that is connecting.
    std::shared_ptr<netWorth::NetworkMazeWorld> world = std::make_shared<netWorth::NetworkMazeWorld>();
    world->AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world->AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
    world->AddAgent<NetWorth::ServerPlayerInterface>("Interface").SetProperty("symbol", '@');

    std::shared_ptr<netWorth::ServerInterface> serverInterface = std::make_shared<netWorth::ServerInterface>();

    world->SetServer(serverInterface);

    sf::UdpSocket * serverSocket = serverInterface->GetSocket();

    std::cout << "Server IP Address: " << sf::IpAddress::getLocalAddress().value() << std::endl;

    //Establish an initial connection
    // Receive a message from anyone
    sf::Packet send_pkt, recv_pkt;
    std::optional<sf::IpAddress> sender;
    unsigned short port;

    if (!serverInterface->InitialConnection(sender, port)) return 1;

    cse491::item_set_t item_set;
    cse491::agent_set_t agent_set;
    std::string input;

    //Main game loop
    while (true) {
        sf::Packet gridPacket = world->GetGridPacket();
        if (serverSocket->send(gridPacket, sender.value(), port) != sf::Socket::Status::Done) {
            std::cout << "Could not send packet" << std::endl;
            return 1;
        }

        if (serverSocket->receive(recv_pkt, sender, port) != sf::Socket::Status::Done) {
            std::cout << "Failure to receive" << std::endl;
            return 1;
        }
        recv_pkt >> input;
        std::cout << input << std::endl;

        if (input == "quit") break;

        world->SetPlayerAction(input);
        world->RunAgents();
    }

    return 0;

}
