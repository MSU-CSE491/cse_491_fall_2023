#include <iostream>
#include <string>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>

using namespace sf;

int main(){
    // ----- The client -----

    // Create a socket (no need to bind it)
    sf::UdpSocket socket;

    // Send a message to the server on port 55002
    std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
    socket.send(message.c_str(), message.size() + 1, "35.9.39.37", 55002); 
    // socket.send(message.c_str(), message.size() + 1, "172.31.20.79", 55002); 

    // Receive an answer (most likely from the server)
    char buffer[1024];
    std::size_t received = 0;
    sf::IpAddress sender;
    unsigned short port = 0; // 0 means any port
    socket.receive(buffer, sizeof(buffer), received, sender, port);
    std::cout << sender.toString() << " said: " << buffer << std::endl;

    return 0;
}