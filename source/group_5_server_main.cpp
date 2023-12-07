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
 * @param start_x Starting x position of client
 * @param start_y Starting y position of client
 * @param world_type Type of world enum
 */
void HandleConnection(netWorth::ServerManager &serverManager, cse491::WorldBase &world, int start_x, int start_y, cse491::WorldType world_type) {
    sf::UdpSocket socket;
    std::optional<sf::IpAddress> sender;
    unsigned short port;

    sf::Packet pkt;
    std::string str;

    // initial connection socket (redirect to serverinterface later)
    if (socket.bind(netWorth::ServerManager::m_initConnectionPort) != sf::Socket::Status::Done){
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
		world.IsWorldRunning(false);

        std::cout << "Connection received from IP Address: " << sender->toString() << " on port: " << port << std::endl;
        pkt >> str;
        std::cout << str << std::endl;

        // Serialize world into string
        std::ostringstream os;
        world.Serialize(os);
        std::string serialized = os.str();
        std::cout << serialized << std::endl;

        serverManager.IncreasePort();

        // send port of server interface, worldtype, x, y, and world data
        pkt.clear();
        pkt << serverManager.m_maxClientPort << static_cast<int>(world_type);
        pkt << start_x << start_y << serialized;
        if (socket.send(pkt, sender.value(), port) != sf::Socket::Status::Done) {
            std::cerr << "Failed to send" << std::endl;
            exit(0);
        }

        // add ServerInterface[port number] to world
        std::string serverInterfaceName = "ServerInterface" + std::to_string(serverManager.m_maxClientPort);
        cse491::Entity & interface = world.AddAgent<netWorth::ServerInterface>
                (serverInterfaceName, "client_ip", sender->toString(), "client_port", port, "server_port",
                 serverManager.m_maxClientPort, "server_manager", &serverManager)
                 .SetProperty("symbol", '@');

        auto & serverInterface = dynamic_cast<netWorth::ServerInterface &>(interface);

        //Do an atomic check to see if you can add it
        serverManager.WriteToActionMap(serverInterface.GetID(), 0);
		serverManager.AddToUpdatePairs(sender.value(), port);
        serverManager.AddToInterfaceSet(serverInterface.GetID());
        serverManager.hasNewAgent = true;

		// serialize agents and send game updates to all clients
		std::ostringstream agent_os;
		world.SerializeAgentSet(agent_os);
		std::string serialized_agents = agent_os.str();
		serverManager.SetSerializedAgents(serialized_agents);
        serverManager.SendGameUpdates();

        std::cout << "Added thread" << std::endl;
    }
}

/**
 * Run demo for MazeWorld type
 * @return 0 if successful, 1 if error
 */
int RunMazeWorldDemo() {
    netWorth::ServerManager manager;

    // Load world
    cse491::MazeWorld world;
    int start_x = 0, start_y = 0;

    // Add agents
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
    auto & astar_agent =
            dynamic_cast<walle::AStarAgent&>(world.AddAgent<walle::AStarAgent>("AStar 1"));
    astar_agent.SetPosition(7, 3);
    astar_agent.SetGoalPosition(21, 7);
    astar_agent.RecalculatePath();

    // Ensure client successfully connects
    std::thread connectionThread(HandleConnection, std::ref(manager), std::ref(world), start_x, start_y, cse491::WorldType::w_maze);
    std::cout << "Handling connection." << std::endl;

    world.RunServer(&manager);
    return 0;
}

/**
 * Run demo for SecondWorld type
 * @return 0 if successful, 1 if error
 */
int RunSecondWorldDemo() {
    netWorth::ServerManager manager;

    // Load world
    group4::SecondWorld world;
    int start_x = 0, start_y = 0;

    // Add agents
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);

    // Add items
    auto powerSword = std::make_unique<cse491::ItemBase>(1, "Sword of Power");
    powerSword->SetProperty("Damage", 20.0);
    powerSword->SetPosition(1, 2);
    world.AddItem(std::move(powerSword));

    // Ensure client successfully connects
    std::thread connectionThread(HandleConnection, std::ref(manager), std::ref(world), start_x, start_y, cse491::WorldType::w_second);
    std::cout << "Handling connection." << std::endl;

    world.RunServer(&manager);
    return 0;
}

/**
 * Run demo for GenerativeWorld type
 * @return 0 if successful, 1 if error
 */
int RunGenerativeWorldDemo() {
    netWorth::ServerManager manager;

    // load world
    static const unsigned int SEED = 973;
    BiomeGenerator biomeGenerator(BiomeType::Maze, 110, 25, SEED);
    biomeGenerator.generate();

    srand(time(NULL));
    auto path = biomeGenerator.clearPath();
    biomeGenerator.applyPathToGrid(path);

    cse491::GenerativeWorld world(SEED);
    int start_x = 0, start_y = 0;

    // Add agents
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);

    // Ensure client successfully connects
    std::thread connectionThread(HandleConnection, std::ref(manager), std::ref(world), start_x, start_y, cse491::WorldType::w_generative);
    std::cout << "Handling connection." << std::endl;

    world.RunServer(&manager);
    return 0;
}

/**
 * Run demo for ManualWorld type
 * @return 0 if successful, 1 if error
 */
int RunManualWorldDemo() {
    netWorth::ServerManager manager;

    // Load world
    cse491_team8::ManualWorld world;
    int start_x = 40, start_y = 3;

    // Add agents
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);

    // Add items
    world.AddItem("Axe", "Chop", 5, "symbol", 'P').SetPosition(37, 3);
    world.AddItem("Boat", "Swim", 7, "symbol", 'U').SetPosition(18, 4);

    // Ensure client successfully connects
    std::thread connectionThread(HandleConnection, std::ref(manager), std::ref(world), start_x, start_y, cse491::WorldType::w_manual);
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

    auto world_type = static_cast<cse491::WorldType>(atoi(argv[1]));
    std::cout<< sf::IpAddress::getLocalAddress()->toString() << std::endl;

    // Run demo based on program args
    if (world_type == cse491::WorldType::w_maze) {
        return RunMazeWorldDemo();
    } else if (world_type == cse491::WorldType::w_second) {
        return RunSecondWorldDemo();
    } else if (world_type == cse491::WorldType::w_generative) {
        return RunGenerativeWorldDemo();
    } else if (world_type == cse491::WorldType::w_manual) {
        return RunManualWorldDemo();
    }

    // World type not in list
    std::cout << "Invalid world type, choose a number between 0 and 3" << std::endl;
    return 1;
}