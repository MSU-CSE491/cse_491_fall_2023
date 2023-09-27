/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate a system.
 * @note Status: PROPOSAL
 **/

// Include the modules that we will be using.
#include "../../TrashInterface.hpp"
#include "../../../Agents/PacingAgent.hpp"
#include "../../../Worlds/MazeWorld.hpp"
#include "../NetworkInterface.hpp"
#include "networkingworld.hpp"

int main()
{
    //Create the world on the server side upon initialization
    //Add the pacing agents to just simply walk around
    //Once everything is added we simply wait for a response from a client that is connecting.
    cse491::netWorth::NetworkMazeWorld world;
    world.AddAgent<cse491::PacingAgent>("Pacer 1").SetPosition(3,1);
    world.AddAgent<cse491::PacingAgent>("Pacer 2").SetPosition(6,1);
    world.AddAgent<cse491::TrashInterface>("Interface").SetProperty("char", '@');
    
    UdpSocket serverSocket;
    serverSocket.bind(55002);

    // Receive a message from anyone
    char buffer[1024];
    std::size_t received = 0;
    sf::IpAddress sender;
    unsigned short port;
    serverSocket.receive(buffer, sizeof(buffer), received, sender, port);
    std::cout << sender.toString() << " said: " << buffer << std::endl;
    
    if(received){
        std::string message = "Pong, my boy!";
        serverSocket.send(message.c_str(), message.size() + 1, sender, port);
        world.Run();
    }
    

    
}