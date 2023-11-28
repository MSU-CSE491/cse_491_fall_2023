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
#include "Interfaces/NetWorth/server/ServerManager.hpp"
#include "core/Entity.hpp"


void ClientThread(netWorth::ServerInterface & interface, cse491::MazeWorld &world,
                  netWorth::ServerManager & serverManager){
    // Send to acknowledge client

    std::cout << "In client thread" << std::endl;

    std::cout << interface.GetName() << std::endl;

    //While this client is still connected (need to fix)
    while (serverManager.ActionMapContains(interface.GetID())){
        std::cout << interface.GetName() << " is connected" << std::endl;
    }
}

/**
 * The initial connection for the server to a client
 * @param sender address of the sender
 * @param port port of the connection
 * @return true if successful
 */
void HandleConnection(netWorth::ServerManager &serverManager, cse491::MazeWorld &world) {
    sf::UdpSocket socket;
    if (socket.bind(netWorth::ServerManager::m_initConnectionPort) != sf::Socket::Status::Done){
        std::cerr << "Failed to bind" << std::endl;
        exit(0);
    }

    sf::Packet pkt;
    std::string str;

    std::optional<sf::IpAddress> sender;
    unsigned short port;

    if (socket.receive(pkt, sender, port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to receive" << std::endl;
        exit(0);
    }

    std::cout << "Connection received from IP Address: " << sender->toString() << " on port: " << port << std::endl;
    pkt >> str;
    std::cout << str << std::endl;


    serverManager.IncreasePort();

    pkt.clear();
    pkt << serverManager.m_maxClientPort;
    if (socket.send(pkt, sender.value(), port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send" << std::endl;
        exit(0);
    }

    std::string serverInterfaceName = "ServerInterface" + std::to_string(serverManager.m_maxClientPort);

    cse491::Entity & interface = world.AddAgent<netWorth::ServerInterface>(serverInterfaceName, "client_ip", sender->toString(),
                                              "client_port", port, "server_port", serverManager.m_maxClientPort)
                                              .SetProperty("symbol", '@');

    auto & serverInterface = dynamic_cast<netWorth::ServerInterface &>(interface);

    //Do an atomic check to see if you can add it
    serverManager.WriteToActionMap(serverInterface.GetID(), 0);

    std::thread clientThread(ClientThread, std::ref(serverInterface), std::ref(world),
                             std::ref(serverManager));

    serverManager.AddToThreadVector(clientThread);
    std::cout << "Added thread" << std::endl;
}

/**
 * Get client connection and send serialized world
 * @param serialized world serialized into a string
 * @return true if successful, else false
 */
bool AwaitClient(const std::string & serialized) {
    // Setup socket
    sf::UdpSocket socket;
    if (socket.bind(55000) != sf::Socket::Status::Done) {
        std::cerr << "Failed to bind socket" << std::endl;
        return false;
    }

    std::cout << sf::IpAddress::getLocalAddress().value() << std::endl;

    // Await client
    sf::Packet recv_pkt, send_pkt;
    std::optional<sf::IpAddress> ip;
    unsigned short port;
    if (socket.receive(recv_pkt, ip, port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to receive" << std::endl;
        return false;
    }
    std::cout << ip.value() << std::endl;
    std::string str;
    recv_pkt >> str;
    std::cout << str << std::endl;

    std::cout << "Server started on IP address: " << sf::IpAddress::getLocalAddress()->toString() << std::endl;


    netWorth::ServerManager serverManager;

    //world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    //world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);

    std::thread connectionThread(HandleConnection, std::ref(serverManager), std::ref(world));
    std::cout << "Handling connection." << std::endl;

    // will probably need to override world Run function for multiple clients
    // assuming we use NetworkMazeWorld rather than MazeWorld
    // that could be difficult for multiple world classes though...
    //serverManager.JoinClients();
    //connectionThread.join();
    // Send serialized world
    send_pkt << serialized;
    if (socket.send(send_pkt, ip.value(), port) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send" << std::endl;
        return false;
    }

    return true;
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
            static_cast<walle::AStarAgent&>(world.AddAgent<walle::AStarAgent>("AStar 1"));
    astar_agent.SetPosition(7, 3);
    astar_agent.SetGoalPosition(21, 7);
    astar_agent.RecalculatePath();

    // Serialize world into string
    std::ostringstream os;
    os << static_cast<int>(cse491::WorldType::w_maze) << ' ' << start_x << ' ' << start_y;
    world.Serialize(os);
    std::string serialized = os.str();
    std::cout << serialized << std::endl;

    // Ensure client successfully connects
    if (!AwaitClient(serialized)) return 1;

    world.AddAgent<netWorth::ServerInterface>("Interface", "server_manager", &manager).SetProperty("symbol", '@').SetPosition(start_x,start_y);
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

    // Serialize world into string
    std::ostringstream os;
    os << static_cast<int>(cse491::WorldType::w_second) << ' ' << start_x << ' ' << start_y;
    world.Serialize(os);
    std::string serialized = os.str();
    std::cout << serialized << std::endl;

    // Ensure client successfully connects
    if (!AwaitClient(serialized)) return 1;

    world.AddAgent<netWorth::ServerInterface>("Interface", "server_manager", &manager).SetProperty("symbol", '@').SetPosition(start_x,start_y);
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

    // Serialize world into string
    std::ostringstream os;
    os << static_cast<int>(cse491::WorldType::w_generative) << ' ' << start_x << ' ' << start_y;
    world.Serialize(os);
    std::string serialized = os.str();
    std::cout << serialized << std::endl;

    // Ensure client successfully connects
    if (!AwaitClient(serialized)) return 1;

    world.AddAgent<netWorth::ServerInterface>("Interface", "server_manager", &manager).SetProperty("symbol", '@').SetPosition(start_x,start_y);
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

    // Serialize world into string
    std::ostringstream os;
    os << static_cast<int>(cse491::WorldType::w_manual) << ' ' << start_x << ' ' << start_y;
    world.Serialize(os);
    std::string serialized = os.str();
    std::cout << serialized << std::endl;

    // Ensure client successfully connects
    if (!AwaitClient(serialized)) return 1;

    world.AddAgent<netWorth::ServerInterface>("Interface", "server_manager", &manager).SetProperty("symbol", '@').SetPosition(start_x,start_y);
    world.RunServer(&manager);

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Must have an argument for world type\nUsage: ./server [number]" << std::endl;
        std::cerr << "0: MazeWorld\n1: SecondWorld\n2: GenerativeWorld\n3: ManualWorld" << std::endl;
        return 1;
    }

    auto world_type = static_cast<cse491::WorldType>(atoi(argv[1]));

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