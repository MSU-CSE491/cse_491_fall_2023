#include <iostream>
//#include "networkingworld.hpp"
#include <SFML/Network.hpp>

int main() {
    sf::IpAddress ip = sf::IpAddress::getLocalAddress();

    std::cout << "success " << ip << std::endl;

    sf::UdpSocket socket;

    // bind the socket to a port
    if (socket.bind(55002) != sf::Socket::Done)
    {
        std::cout << "Failure to bind socket" << std::endl;
        return 1;
    }

    char data[100];
    std::size_t received;

    // UDP socket:
    sf::IpAddress sender;
    unsigned short port;
    if (socket.receive(data, 100, received, sender, port) != sf::Socket::Done)
    {
        std::cout << "Failure to receive" << std::endl;
        return 1;
    }

    std::cout << "Received " << received << " bytes from " << sender << " on port " << port << std::endl;
    for (int i = 0; data[i] && i < 100; i++) {
        printf("%c", data[i]);
    }

    // send back
    std::string message = "ping!";
    if (socket.send(message.c_str(), message.size(), sender, port) != sf::Socket::Done)
    {
        std::cout << "Failure to receive" << std::endl;
        return 1;
    }

    return 0;
}