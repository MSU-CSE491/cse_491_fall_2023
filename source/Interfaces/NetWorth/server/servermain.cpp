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
#include "ServerInterface.hpp"
#include "networkingworld.hpp"

Packet GridToPacket(const cse491::WorldGrid & grid, const cse491::type_options_t & type_options,
                  const cse491::item_set_t & item_set, const cse491::agent_set_t & agent_set)
    {
    std::vector<std::string> packet_grid(grid.GetHeight());

    // Load the world into the symbol_grid;
    for (size_t y=0; y < grid.GetHeight(); ++y) {
    packet_grid[y].resize(grid.GetWidth());
        for (size_t x=0; x < grid.GetWidth(); ++x) {
            packet_grid[y][x] = type_options[ grid.At(x,y) ].symbol;
        }
    }

    // Add in the agents / entities
    for (const auto & entity_ptr : item_set) {
    cse491::GridPosition pos = entity_ptr->GetPosition();
    packet_grid[pos.CellY()][pos.CellX()] = '+';
    }

    for (const auto & agent_ptr : agent_set) {
    cse491::GridPosition pos = agent_ptr->GetPosition();
    char c = '*';
    if(agent_ptr->HasProperty("char")){
        c = static_cast<char>(agent_ptr->GetProperty("char"));
    }
    packet_grid[pos.CellY()][pos.CellX()] = c;
    }


    // Print out the symbol_grid with a box around it.
    std::ostringstream oss;
    oss << '+' << std::string(grid.GetWidth(),'-') << "+\n";
    for (const auto & row : packet_grid) {
    oss << "|";
    for (char cell : row) {
        // std::cout << ' ' << cell;
        oss << cell;
    }
    oss << "|\n";
    }
    oss << '+' << std::string(grid.GetWidth(),'-') << "+\n";
    oss << "\nYour move? ";
    std::string gridString = oss.str();

    Packet gridPacket;
    gridPacket << gridString;

    return gridPacket;
}

int main() {
    //Create the world on the server side upon initialization
    //Add the pacing agents to just simply walk around
    //Once everything is added we simply wait for a response from a client that is connecting.
    cse491::netWorth::NetworkMazeWorld world;
    //Populate world with PacingAgents
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);

    cse491::netWorth::ServerInterface serverInterface;

    //Not going to add TrashInterface here, client will do that once it gets the world
//    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');

    //Endless server running
    //Wait for initial connection
    std::cout << "Server IP: " << sf::IpAddress::getLocalAddress() << std::endl;

    serverInterface.GetSocket()->bind(55002);

    IpAddress clientIP;

    //Gets clientIP from connection
    while (true) {
        clientIP = serverInterface.ReceivePacket();
        if (clientIP.toString() != "0.0.0.0") {
            break;
        }
    }

    std::cout << "Got Client IP. IP address: " << clientIP << std::endl;

    Packet recv_pkt;
    IpAddress sender;
    unsigned short port;
    std::string str;
//    while (true) {
        cse491::item_set_t item_set;
        cse491::agent_set_t agent_set;
        size_t input;

        while (true) {
            std::cout << "before gridpacket send" << std::endl;
            sf::Packet gridPacket = GridToPacket(world.GetGrid(), world.GetCellTypes(), item_set, agent_set);
            serverInterface.GetSocket()->send(gridPacket, sender, port);
            std::cout << "after gridpacket send" << std::endl;

            std::cout << "before serverinterface receive" << std::endl;
            serverInterface.GetSocket()->receive(recv_pkt, sender, port);
            std::cout << "after serverinterface receive" << std::endl;
            recv_pkt >> str;
            std::cout << str;
        }


//    while (true){
//
////        break;
//    }
//    UdpSocket serverSocket;
//    serverSocket.bind(55002);
//
//    std::cout << sf::IpAddress::getLocalAddress() << std::endl;
//
//    // Receive a message from anyone
//    char buffer[1024];
//    std::size_t received = 0;
//    sf::IpAddress sender;
//    unsigned short port;
//    serverSocket.receive(buffer, sizeof(buffer), received, sender, port);
//    std::cout << sender.toString() << " said: " << buffer << std::endl;
//
//    if(received){
//        std::string message = "Pong!";
//        serverSocket.send(message.c_str(), message.size() + 1, sender, port);
//        //world.NetworkRun(&serverSocket, sender, port);
//        cse491::item_set_t item_set;
//        cse491::agent_set_t agent_set;
//        sf::Packet gridPacket = GridToPacket(world.GetGrid(), world.GetCellTypes(), item_set, agent_set);
//        serverSocket.send(gridPacket, sender, port);
//    }

//    }
}