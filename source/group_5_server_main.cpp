/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.

#include "Agents/PacingAgent.hpp"
#include "Interfaces/NetWorth/server/ServerInterface.hpp"
#include "Worlds/MazeWorld.hpp"

int main() {
    std::cout << sf::IpAddress::getLocalAddress().value() << std::endl;

    cse491::MazeWorld world;
    sf::UdpSocket socket;
    if (socket.bind(55000) != sf::Socket::Status::Done) {
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }

    std::ostringstream os;
    world.Serialize(os);
    std::string serialized = os.str();

//    std::cout << serialized << std::endl;
//
//    std::istringstream is(serialized);
//    world.Deserialize(is);

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
    send_pkt << serialized;

    if (socket.send(send_pkt, ip.value(), port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send" << std::endl;
        return 1;
    }


    //world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    //world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
    world.AddAgent<netWorth::ServerInterface>("Interface").SetProperty("symbol", '@');

    // will probably need to override world Run function for multiple clients
    // assuming we use NetworkMazeWorld rather than MazeWorld
    // that could be difficult for multiple world classes though...
    world.Run();
    return 0;

}

