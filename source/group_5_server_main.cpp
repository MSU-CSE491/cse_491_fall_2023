/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include <thread>
#include <mutex>
#include <SFML/Network.hpp>
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
#include "core/Entity.hpp"

/**
 * Initial connection for server to client
 * @param serverManager Server manager
 * @param world World to serialize and add interfaces to
 * @param startX Starting x position of client
 * @param startY Starting y position of client
 * @param worldType Type of world enum
 */
void handleConnection(netWorth::ServerManager &serverManager, cse491::WorldBase &world, int startX, int startY, cse491::WorldType worldType) {
    sf::UdpSocket socket;
    std::optional<sf::IpAddress> sender;
    unsigned short port;

    sf::Packet pkt;
    std::string str;

    // initial connection socket (redirect to serverinterface later)
    if (socket.bind(netWorth::ServerManager::m_init_connection_port) != sf::Socket::Status::Done){
        std::cerr << "Failed to bind" << std::endl;
        exit(0);
    }

    while (true){
        // await client
        if (socket.receive(pkt, sender, port) != sf::Socket::Status::Done) {
            std::cerr << "Failed to receive" << std::endl;
            exit(0);
        }

        // stop agents from running while dealing with new client
        world.SetWorldRunning(false);

        std::cout << "Connection received from IP Address: " << sender->toString() << " on port: " << port << std::endl;
        pkt >> str;
        std::cout << str << std::endl;

        // Serialize world into string
        std::ostringstream os;
        world.Serialize(os);
        std::string serialized = os.str();
        std::cout << serialized << std::endl;

		serverManager.increasePort();

        // send port of server interface, world type, x, y, and world data
        pkt.clear();
        pkt << serverManager.m_max_client_port << static_cast<int>(worldType);
        pkt << startX << startY << serialized;
        if (socket.send(pkt, sender.value(), port) != sf::Socket::Status::Done) {
            std::cerr << "Failed to send" << std::endl;
            exit(0);
        }

        // add ServerInterface[port number] to world
        std::string serverInterfaceName = "ServerInterface" + std::to_string(serverManager.m_max_client_port);
        cse491::Entity & interface = world.AddAgent<netWorth::ServerInterface>
                (serverInterfaceName, "client_ip", sender->toString(), "client_port", port, "server_port",
                 serverManager.m_max_client_port, "server_manager", &serverManager)
                 .SetProperty("symbol", '@');

        auto & serverInterface = dynamic_cast<netWorth::ServerInterface &>(interface);

        //Do an atomic check to see if you can add it
		serverManager.writeToActionMap(serverInterface.GetID(), 0);
		serverManager.addToUpdatePairs(sender.value(), port);
		serverManager.addToInterfaceSet(serverInterface.GetID());
		serverManager.setNewAgent(true);

		// serialize agents and send game updates to all clients
		std::ostringstream agent_os;
		world.SerializeAgentSet(agent_os);
		std::string serialized_agents = agent_os.str();
		serverManager.setSerializedAgents(serialized_agents);
		serverManager.sendGameUpdates();

        std::cout << "Added thread" << std::endl;
    }
}

/**
 * Run demo for MazeWorld type
 * @return 0 if successful, 1 if error
 */
int runMazeWorldDemo() {
    netWorth::ServerManager manager;

    // Load world
    cse491::MazeWorld world;
    int startX = 0, startY = 0;

    // Add agents
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
    auto & aStarAgent =
            dynamic_cast<walle::AStarAgent&>(world.AddAgent<walle::AStarAgent>("AStar 1"));
    aStarAgent.SetPosition(7, 3);
    aStarAgent.SetGoalPosition(21, 7);
    aStarAgent.RecalculatePath();

    // Ensure client successfully connects
    std::thread connectionThread(handleConnection, std::ref(manager), std::ref(world), startX, startY, cse491::WorldType::w_maze);
    std::cout << "Handling connection." << std::endl;

    world.RunServer(&manager);
    return 0;
}

/**
 * Run demo for SecondWorld type
 * @return 0 if successful, 1 if error
 */
int runSecondWorldDemo() {
    netWorth::ServerManager manager;

    // Load world
    group4::SecondWorld world;
    int startX = 0, startY = 0;

    // Add agents
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);

    // Add items
    auto powerSword = std::make_unique<cse491::ItemBase>(1, "Sword of Power");
    powerSword->SetProperty("Damage", 20.0);
    powerSword->SetPosition(1, 2);
    world.AddItem(std::move(powerSword));

    // Ensure client successfully connects
    std::thread connectionThread(handleConnection, std::ref(manager), std::ref(world), startX, startY, cse491::WorldType::w_second);
    std::cout << "Handling connection." << std::endl;

    world.RunServer(&manager);
    return 0;
}

/**
 * Run demo for GenerativeWorld type
 * @return 0 if successful, 1 if error
 */
int runGenerativeWorldDemo() {
    netWorth::ServerManager manager;

    // load world
    static const unsigned int SEED = 973;
    group6::BiomeGenerator biomeGenerator(group6::BiomeType::Maze, 110, 25, SEED);
    biomeGenerator.generate();

    srand(time(NULL));
    auto path = biomeGenerator.clearPath();
    biomeGenerator.applyPathToGrid(path);

    group6::GenerativeWorld world(group6::BiomeType::Maze, 110, 25, SEED);
    int startX = 0, startY = 0;

    // Add agents
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);

    // Ensure client successfully connects
    std::thread connectionThread(handleConnection, std::ref(manager), std::ref(world), startX, startY, cse491::WorldType::w_generative);
    std::cout << "Handling connection." << std::endl;

    world.RunServer(&manager);
    return 0;
}

/**
 * Run demo for ManualWorld type
 * @return 0 if successful, 1 if error
 */
int runManualWorldDemo() {
    netWorth::ServerManager manager;

    // Load world
    cse491_team8::ManualWorld world;
    int startX = 40, startY = 3;

    // Add agents
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);

    // Add items
    world.AddItem("Axe", "Chop", 5, "symbol", 'P').SetPosition(37, 3);
    world.AddItem("Boat", "Swim", 7, "symbol", 'U').SetPosition(18, 4);

    // Ensure client successfully connects
    std::thread connectionThread(handleConnection, std::ref(manager), std::ref(world), startX, startY, cse491::WorldType::w_manual);
    std::cout << "Handling connection." << std::endl;

    world.RunServer(&manager);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Must have an argument for world type\nUsage: ./server [number]" << std::endl;
        std::cerr << "0: MazeWorld\n1: SecondWorld\n2: GenerativeWorld\n3: ManualWorld" << std::endl;
        return 1;
    }

    auto worldType = static_cast<cse491::WorldType>(atoi(argv[1]));
    std::cout<< sf::IpAddress::getLocalAddress()->toString() << std::endl;

    // Run demo based on program args
    if (worldType == cse491::WorldType::w_maze) {
        return runMazeWorldDemo();
    } else if (worldType == cse491::WorldType::w_second) {
        return runSecondWorldDemo();
    } else if (worldType == cse491::WorldType::w_generative) {
        return runGenerativeWorldDemo();
    } else if (worldType == cse491::WorldType::w_manual) {
        return runManualWorldDemo();
    }

    // World type not in list
    std::cout << "Invalid world type, choose a number between 0 and 3" << std::endl;
    return 1;
}
