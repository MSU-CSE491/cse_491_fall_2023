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
#include "Worlds/SecondWorld.hpp"
#include "Worlds/BiomeGenerator.hpp"
#include "Worlds/GenerativeWorld.hpp"
#include "Worlds/ManualWorld.hpp"

unsigned short clientKillPort;

std::string clientKillIP;

sf::UdpSocket * clientKillSocket = nullptr;

void TerminateClient() {
    // Request connection to server
    sf::Packet send_pkt;
    std::optional<sf::IpAddress> ip_addr = sf::IpAddress::resolve(clientKillIP);
    send_pkt << static_cast<size_t>(9999);
    if (clientKillSocket->send(send_pkt, ip_addr.value(), clientKillPort) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send" << std::endl;
    }
}

/**
 * Run Maze World client instance
 * @param is istream to deserialize
 * @param ip_string IP address of server
 * @param start_x x start position
 * @param start_y y start position
 * @return true if successful
 */
bool RunMazeWorldDemo(std::istream &is, const std::string &ip_string, unsigned short port, int start_x, int start_y, sf::UdpSocket *socket) {
    netWorth::ClientManager manager;
    std::string interface_name = "Interface1";
    cse491::MazeWorld world;
    world.Deserialize(is, &manager);
    clientKillPort = port;
    clientKillIP = ip_string;
    cse491::Entity & interface = world.AddAgent<netWorth::ClientInterface>(interface_name, "server_ip", ip_string,
                                              "server_port", port, "manager", &manager,
                                              "socket", socket)
                                              .SetProperty("symbol", '@')
                                              .SetPosition(start_x, start_y);

    auto & clientInterface = dynamic_cast<netWorth::ClientInterface &>(interface);

    clientKillSocket = clientInterface.GetSocket();
    world.Run();
    return true;
}

/**
 * Run Second World client instance
 * @param is istream to deserialize
 * @param ip_string IP address of server
 * @param start_x x start position
 * @param start_y y start position
 * @return true if successful
 */
bool RunSecondWorldDemo(std::istream &is, const std::string &ip_string, unsigned short port, int start_x, int start_y) {
    netWorth::ClientManager manager;
    std::string interface_name = "Interface";
    group4::SecondWorld world;
    world.Deserialize(is, &manager);

    world.AddAgent<netWorth::ClientInterface>(interface_name, "ip", ip_string,
                                              "port", port, "manager", &manager)
                                              .SetProperty("symbol", '@')
                                              .SetPosition(start_x, start_y);
    world.Run();
    return true;
}

/**
 * Run Generative World client instance
 * @param is istream to deserialize
 * @param ip_string IP address of server
 * @param start_x x start position
 * @param start_y y start position
 * @return true if successful
 */
bool RunGenerativeWorldDemo(std::istream &is, const std::string &ip_string, unsigned short port, int start_x, int start_y) {
    netWorth::ClientManager manager;
    std::string interface_name = "Interface2";
    cse491::GenerativeWorld world;
    world.Deserialize(is, &manager);

    world.AddAgent<netWorth::ClientInterface>(interface_name, "server_ip", ip_string,
                                              "server_port", port, "manager", &manager)
                                              .SetProperty("symbol", '@')
                                              .SetPosition(start_x, start_y);
    world.Run();
    return true;
}

/**
 * Run Manual World client instance
 * @param is istream to deserialize
 * @param ip_string IP address of server
 * @param start_x x start position
 * @param start_y y start position
 * @return true if successful
 */
bool RunManualWorldDemo(std::istream &is, const std::string &ip_string, unsigned short port, int start_x, int start_y) {
    netWorth::ClientManager manager;
    std::string interface_name = "Interface3";
    cse491_team8::ManualWorld world;
    world.Deserialize(is, &manager);

    cse491::Entity & interface = world.AddAgent<netWorth::ClientInterface>(interface_name, "ip", ip_string,
                                              "port", port, "manager", &manager)
                                              .SetProperty("symbol", '@')
                                              .SetPosition(start_x, start_y);

    auto & clientInterface = dynamic_cast<netWorth::ClientInterface &>(interface);

    world.Run();
    return true;
}

/**
 * Main function
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Must have an argument for server IP\nUsage: ./client [IP]" << std::endl;
        return 1;
    }

    std::atexit(TerminateClient);

    std::string ip_string(argv[1]);
    // port is hardcoded, 55000 will be the initial connection port
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
    recv_pkt >> port >> serialized;
    std::istringstream is(serialized);
    int world_type_int, start_x, start_y;
    is >> world_type_int >> start_x >> start_y;
    auto world_type = static_cast<cse491::WorldType>(world_type_int);

    // TODO: Find a better way to deal with worlds other than commenting/uncommenting
    // Will probably find a way to determine world type from server
    // Note that interface names must be different to properly load textures
    // Will probably also send start position instead of hard-coding
    if (world_type == cse491::WorldType::w_maze) {
        return RunMazeWorldDemo(is, ip_string, port, start_x, start_y, &socket);
    } else if (world_type == cse491::WorldType::w_second) {
        return RunSecondWorldDemo(is, ip_string, port, start_x, start_y);
    } else if (world_type == cse491::WorldType::w_generative) {
        return RunGenerativeWorldDemo(is, ip_string, port, start_x, start_y);
    } else if (world_type == cse491::WorldType::w_manual) {
        return RunManualWorldDemo(is, ip_string, port, start_x, start_y);
    }

    return 0;
}
