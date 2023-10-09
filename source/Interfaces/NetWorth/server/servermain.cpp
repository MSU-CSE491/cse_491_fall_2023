/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include <sstream>
#include<string>
#include<vector>
#include "../../../core/Data.hpp"
#include "../../TrashInterface.hpp"
#include "../../../Agents/PacingAgent.hpp"
#include "../../../Worlds/MazeWorld.hpp"
#include "../NetworkInterface.hpp"
#include "networkingworld.hpp"

int main()
{
    //Create the world on the server side upon initialization
    //Add the pacing agents to just simply walk around
    //Once everything is added we simply wait for a response from a client that is connecting.
    std::shared_ptr<cse491::netWorth::NetworkMazeWorld> world = std::make_shared<cse491::netWorth::NetworkMazeWorld>();
    world->AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world->AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
    world->AddAgent<cse491::TrashInterface>("Interface").SetProperty("symbol", 'Z');

    std::shared_ptr<cse491::netWorth::ServerInterface> serverInterface = std::make_shared<cse491::netWorth::ServerInterface>();

    world->SetServer(serverInterface);

    UdpSocket * serverSocket = serverInterface->GetSocket();

    std::cout << "Server IP Address: " << sf::IpAddress::getLocalAddress() << std::endl;

    //Establish an initial connection
    // Receive a message from anyone
    sf::Packet send_pkt;
    sf::Packet recv_pkt;
    std::string str;

    sf::IpAddress sender;
    unsigned short port;
    serverInterface->InitialConnection(sender, send_pkt, recv_pkt, port, str);

    cse491::item_set_t item_set;
    cse491::agent_set_t agent_set;
    std::string input;

    //Main game loop
    while (true) {
        sf::Packet gridPacket = world->GetGridPacket();
        serverSocket->send(gridPacket, sender, port);

        serverSocket->receive(recv_pkt, sender, port);
        recv_pkt >> input;
        std::cout << input << std::endl;

        if (input == "quit") break;
    }

    return 0;
    
}