/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A client-side networking main to prototype communication
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.

#include "Interfaces/NetWorth/client/ClientInterface.hpp"
#include "Interfaces/NetWorth/client/ControlledAgent.hpp"
#include "Interfaces/NetWorth/client/ClientManager.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Agents/PacingAgent.hpp"
#include "Worlds/SecondWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"
#include "Worlds/GenerativeWorld.hpp"
#include "Worlds/ManualWorld.hpp"

/**
 * Deserialize agents and add to world
 * @param is istream
 * @param world world that is being added to
 * @param manager pointer to ClientManager for agents
 */
void DeserializeAgentSet(std::istream &is, cse491::WorldBase &world, netWorth::ClientManager *manager) {
    // find beginning of agent_set serialization
    std::string read;
    std::getline(is, read, '\n');
    if (read != ":::START agent_set") {
        std::cerr << "Could not find start of agent_set serialization" << std::endl;
        return;
    }

    std::string name, x, y;
    size_t size;

    // how many agents?
    std::getline(is, read, '\n');
    size = stoi(read);

    // read each agent (only deserialize name, x, and y for now)
    for (size_t i = 0; i < size; i++) {
        std::getline(is, name, '\n');
        std::getline(is, x, '\n');
        std::getline(is, y, '\n');

        world.AddAgent<netWorth::ControlledAgent>(name, "manager", manager).SetPosition(stoi(x), stoi(y));
    }

    std::getline(is, read, '\n');
    if (read != ":::END agent_set") {
        std::cerr << "Could not find end of agent_set serialization" << std::endl;
        return;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Must have an argument for server IP\nUsage: ./client [IP]" << std::endl;
        return 1;
    }

    std::string ip_string(argv[1]);
    unsigned short port = 55000;

    // Request connection to server
    netWorth::ClientManager manager;
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
    std::istringstream is(serialized);

    // TODO: Find a better way to deal with worlds other than commenting/uncommenting
    // Will probably find a way to determine world type from server
    // Note that interface names must be different to properly load textures
    // Will probably also send start position instead of hard-coding
//    std::string interface_name = "Interface1";
//    cse491::MazeWorld world;
//    world.Deserialize(is);
//    int start_x = 0, start_y = 0;

//    std::string interface_name = "Interface";
//    group4::SecondWorld world;
//    world.Deserialize(is);
//    int start_x = 0, start_y = 0;

//    std::string interface_name = "Interface2";
//    cse491::GenerativeWorld world;
//    world.Deserialize(is);
//    int start_x = 0, start_y = 0;

    std::string interface_name = "Interface3";
    cse491_team8::ManualWorld world;
    world.Deserialize(is);
    int start_x = 40, start_y = 3;

    port = 55002;

    DeserializeAgentSet(is, world, &manager);
    world.DeserializeItemSet(is);
    world.AddAgent<netWorth::ClientInterface>(interface_name, "ip", ip_string, "port", port, "manager", &manager).SetProperty("symbol", '@').SetPosition(start_x, start_y);

    world.Run();
    return 0;
}
