/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A client-side networking main to prototype communication
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.

#include "Interfaces/NetWorth/client/ClientInterface.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Agents/PacingAgent.hpp"
#include "Worlds/SecondWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"
#include "Worlds/GenerativeWorld.hpp"
#include "Worlds/ManualWorld.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Must have an argument for server IP\nUsage: ./client [IP]" << std::endl;
        return 1;
    }

    std::string ip_string(argv[1]);
    unsigned short port = 55000;

    // Request connection to server
    sf::UdpSocket socket;
    sf::Packet send_pkt, recv_pkt;
    std::optional<sf::IpAddress> ip_addr = sf::IpAddress::resolve(ip_string);
    std::string serialized;

    send_pkt << "Requesting connection";
    if (socket.send(send_pkt, ip_addr.value(), port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send" << std::endl;
        return 1;
    }

    // Receive world and deserialize
    if (socket.receive(recv_pkt, ip_addr, port) !=  sf::Socket::Status::Done) {
        std::cerr << "Failed to receive" << std::endl;
        return 1;
    }
    recv_pkt >> serialized;

    // TODO: Find a better way to deal with worlds other than commenting/uncommenting
    // Will probably find a way to determine world type from server
    // Note that interface names must be different to properly load textures
    // Will probably also send start position instead of hard-coding
    std::string interface_name = "Interface1";
    cse491::MazeWorld world(serialized);
    int start_x = 0, start_y = 0;

//    std::string interface_name = "Interface";
//    group4::SecondWorld world(serialized);
//    int start_x = 0, start_y = 0;

//    std::string interface_name = "Interface2";
//    cse491::GenerativeWorld world(serialized);
//    int start_x = 0, start_y = 0;

//    std::string interface_name = "Interface3";
//    cse491_team8::ManualWorld world(serialized);
//    int start_x = 40, start_y = 3;

    port = 55002;

    world.AddAgent<netWorth::ClientInterface>(interface_name, "ip", ip_string, "port", port).SetProperty("symbol", '@').SetPosition(start_x, start_y);
    //world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3, 1);
    //world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6, 1);

    world.Run();
    return 0;
}
