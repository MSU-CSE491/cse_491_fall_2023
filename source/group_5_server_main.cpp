/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.

#include "Agents/PacingAgent.hpp"
#include "Interfaces/NetWorth/server/ServerInterface.hpp"
#include "Interfaces/NetWorth/server/ServerManager.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"
#include "Agents/PathAgent.hpp"
#include "Worlds/GenerativeWorld.hpp"
#include "Worlds/ManualWorld.hpp"
#include "Worlds/SecondWorld.hpp"
#include "Agents/AStarAgent.hpp"

int main() {
    std::cout << sf::IpAddress::getLocalAddress().value() << std::endl;

    netWorth::ServerManager manager;
    sf::UdpSocket socket;
    if (socket.bind(55000) != sf::Socket::Status::Done) {
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }

    // TODO: Find a better way to deal with worlds other than commenting/uncommenting

    cse491::MazeWorld world;
    int start_x = 0, start_y = 0;

//    group4::SecondWorld world;
//    int start_x = 0, start_y = 0;

//    static const unsigned int SEED = 973;
//    BiomeGenerator biomeGenerator(BiomeType::Maze, 110, 25, SEED);
//    biomeGenerator.generate();
//
//    srand(time(NULL));
//    auto path = biomeGenerator.clearPath();
//    biomeGenerator.applyPathToGrid(path);
//
//    cse491::GenerativeWorld world(SEED);
//    int start_x = 0, start_y = 0;

//    cse491_team8::ManualWorld world;
//    int start_x = 40, start_y = 3;

    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
    auto & astar_agent =
            static_cast<walle::AStarAgent&>(world.AddAgent<walle::AStarAgent>("AStar 1"));
    astar_agent.SetPosition(7, 3);
    astar_agent.SetGoalPosition(20, 8);
    astar_agent.RecalculatePath();

    // Serialize world into string
    std::ostringstream os;
    world.Serialize(os);
    std::string serialized = os.str();

    // Await client
    sf::Packet recv_pkt, send_pkt;
    std::optional<sf::IpAddress> ip;
    unsigned short port;
    if (socket.receive(recv_pkt, ip, port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to receive" << std::endl;
        return 1;
    }
    std::cout << ip.value() << std::endl;
    std::string str;
    recv_pkt >> str;
    std::cout << str << std::endl;

    // Send serialized world
    send_pkt << serialized;
    if (socket.send(send_pkt, ip.value(), port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send" << std::endl;
        return 1;
    }

    world.AddAgent<netWorth::ServerInterface>("Interface", "server_manager", &manager).SetProperty("symbol", '@').SetPosition(start_x,start_y);
    // TODO: Serialize agents after server interface added?


    world.SetManager(&manager);
    world.Run();
    return 0;

}

