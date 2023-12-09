/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A client-side networking main to prototype communication
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.

#include "Interfaces/NetWorth/client/ClientInterface.hpp"
#include "Interfaces/NetWorth/client/ClientManager.hpp"
#include "Worlds/MazeWorld.hpp"
#include "Worlds/SecondWorld.hpp"
#include "Worlds/GenerativeWorld.hpp"
#include "Worlds/ManualWorld.hpp"

unsigned short clientKillPort;

std::string clientKillIP;

sf::UdpSocket * clientKillSocket = nullptr;

void terminateClient() {
    // Request connection to server
    sf::Packet sendPkt;
    std::optional<sf::IpAddress> ipAddr = sf::IpAddress::resolve(clientKillIP);
    sendPkt << static_cast<size_t>(9999);
    if (clientKillSocket->send(sendPkt, ipAddr.value(), clientKillPort) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send" << std::endl;
    }
}

/**
 * Run Maze World client instance
 * @param is istream to deserialize
 * @param ipString IP address of server
 * @param startX x start position
 * @param startY y start position
 * @return true if successful
 */
bool runMazeWorldDemo(std::istream &is, const std::string &ipString, unsigned short port, int startX, int startY,
	sf::UdpSocket *socket, unsigned short initPort) {
    netWorth::ClientManager manager;
	manager.setUpdatePort(initPort);
	manager.setupGameUpdateSocket(socket);
    std::string interfaceName = "Interface1";
    cse491::MazeWorld world;
    world.Deserialize(is, &manager);
    clientKillPort = port;
    clientKillIP = ipString;
    cse491::Entity & interface = world.AddAgent<netWorth::ClientInterface>(interfaceName, "server_ip", ipString,
                                              "server_port", port, "manager", &manager,
                                              "socket", socket)
                                              .SetProperty("symbol", '@')
                                              .SetPosition(startX, startY);

    auto & clientInterface = dynamic_cast<netWorth::ClientInterface &>(interface);

    clientKillSocket = clientInterface.getSocket();
    world.RunClient(&manager);
    return true;
}

/**
 * Run Second World client instance
 * @param is istream to deserialize
 * @param ipString IP address of server
 * @param startX x start position
 * @param startY y start position
 * @return true if successful
 */
bool runSecondWorldDemo(std::istream &is, const std::string &ipString, unsigned short port, int startX, int startY,
	sf::UdpSocket *socket, unsigned short initPort) {
	netWorth::ClientManager manager;
	manager.setUpdatePort(initPort);
	manager.setupGameUpdateSocket(socket);
	std::string interfaceName = "Interface";
	group4::SecondWorld world;;
	world.Deserialize(is, &manager);
	clientKillPort = port;
	clientKillIP = ipString;
	cse491::Entity & interface = world.AddAgent<netWorth::ClientInterface>(interfaceName, "server_ip", ipString,
			"server_port", port, "manager", &manager,
			"socket", socket)
		.SetProperty("symbol", '@')
		.SetPosition(startX, startY);

	auto & clientInterface = dynamic_cast<netWorth::ClientInterface &>(interface);

	clientKillSocket = clientInterface.getSocket();
	world.RunClient(&manager);
	return true;
}

/**
 * Run Generative World client instance
 * @param is istream to deserialize
 * @param ipString IP address of server
 * @param startX x start position
 * @param startY y start position
 * @return true if successful
 */
bool runGenerativeWorldDemo(std::istream &is, const std::string &ipString, unsigned short port, int startX,
	int startY, sf::UdpSocket *socket, unsigned short initPort) {
	netWorth::ClientManager manager;
	manager.setUpdatePort(initPort);
	manager.setupGameUpdateSocket(socket);
	std::string interfaceName = "Interface2";
  auto biome = group6::BiomeType::Maze;       // change world biome type here
  int width = 110;                            // change world width here
  int height = 25;                            // change world height here
  static const unsigned int SEED = 973;       // change world seed here
  group6::GenerativeWorld world(biome, width, height, SEED);
	world.Deserialize(is, &manager);
	clientKillPort = port;
	clientKillIP = ipString;
	cse491::Entity & interface = world.AddAgent<netWorth::ClientInterface>(interfaceName, "server_ip", ipString,
			"server_port", port, "manager", &manager,
			"socket", socket)
		.SetProperty("symbol", '@')
		.SetPosition(startX, startY);

	auto & clientInterface = dynamic_cast<netWorth::ClientInterface &>(interface);

	clientKillSocket = clientInterface.getSocket();
	world.RunClient(&manager);
	return true;
}

/**
 * Run Manual World client instance
 * @param is istream to deserialize
 * @param ipString IP address of server
 * @param startX x start position
 * @param startY y start position
 * @return true if successful
 */
bool runManualWorldDemo(std::istream &is, const std::string &ipString, unsigned short port, int startX, int startY,
	sf::UdpSocket *socket, unsigned short initPort) {
	netWorth::ClientManager manager;
	manager.setUpdatePort(initPort);
	manager.setupGameUpdateSocket(socket);
	std::string interface_name = "Interface3";
	cse491_team8::ManualWorld world;
	world.Deserialize(is, &manager);
	clientKillPort = port;
	clientKillIP = ipString;
	cse491::Entity & interface = world.AddAgent<netWorth::ClientInterface>(interface_name, "server_ip", ipString,
			"server_port", port, "manager", &manager,
			"socket", socket)
		.SetProperty("symbol", '@')
		.SetPosition(startX, startY);

	auto & clientInterface = dynamic_cast<netWorth::ClientInterface &>(interface);
  dynamic_cast<i_2D::MainInterface&>(interface).SetLargeGrid(true);

	clientKillSocket = clientInterface.getSocket();
	world.RunClient(&manager);
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

	std::atexit(terminateClient);

    std::string ipString(argv[1]);
    // port is hardcoded, 55000 will be the initial connection port
    unsigned short port = 55000;

    // Request connection to server
    sf::UdpSocket socket;
    sf::Packet sendPkt, recvPkt;
    std::optional<sf::IpAddress> ipAddr = sf::IpAddress::resolve(ipString);
    std::string serialized;

    sendPkt << "Requesting connection";
    if (socket.send(sendPkt, ipAddr.value(), port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send" << std::endl;
        return 1;
    }

    // Receive world and deserialize
    if (socket.receive(recvPkt, ipAddr, port) !=  sf::Socket::Status::Done) {
        std::cerr << "Failed to receive" << std::endl;
        return 1;
    }

	unsigned short initPort = socket.getLocalPort();

    recvPkt >> port;
    int worldTypeInt, startX, startY;
    recvPkt >> worldTypeInt >> startX >> startY;
    recvPkt >> serialized;
    std::istringstream is(serialized);
    auto worldType = static_cast<cse491::WorldType>(worldTypeInt);

    // TODO: Find a better way to deal with worlds other than commenting/uncommenting
    // Will probably find a way to determine world type from server
    // Note that interface names must be different to properly load textures
    // Will probably also send start position instead of hard-coding
    if (worldType == cse491::WorldType::w_maze) {
        return runMazeWorldDemo(is, ipString, port, startX, startY, &socket, initPort);
    } else if (worldType == cse491::WorldType::w_second) {
        return runSecondWorldDemo(is, ipString, port, startX, startY, &socket, initPort);
    } else if (worldType == cse491::WorldType::w_generative) {
        return runGenerativeWorldDemo(is, ipString, port, startX, startY, &socket, initPort);
    } else if (worldType == cse491::WorldType::w_manual) {
        return runManualWorldDemo(is, ipString, port, startX, startY, &socket, initPort);
    }

    return 0;
}
