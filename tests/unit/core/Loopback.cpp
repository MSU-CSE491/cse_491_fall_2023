/*
 * @author Ethan Rylko
 * @brief Unit tests for simple SFML networking functions
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "SFML/Network.hpp"

TEST_CASE("Loopback", "[networth]"){
    sf::UdpSocket socket;
    unsigned short port = 50000;

    // Data to send and receive buffers
    // For plain data (not packets), size_t is bytes received
    std::string msg = "test";
    char recv_buf[5];
    size_t received;

    // Bind port, check IP not null, send, receive, check data
    // lol imagine if your own IP returned null
    CHECK(socket.bind(port) == sf::Socket::Status::Done);
    std::optional<sf::IpAddress> ip = sf::IpAddress::getLocalAddress();
    CHECK(ip);      // std::optional<IpAddress>, ensure not null
    CHECK(socket.send(msg.c_str(), msg.size() + 1, ip.value(), port) == sf::Socket::Status::Done);
    CHECK(socket.receive(recv_buf, sizeof(recv_buf), received, ip, port) == sf::Socket::Status::Done);
    CHECK(received == msg.size() + 1);
    CHECK(std::string(recv_buf) == msg);
}

TEST_CASE("PacketLoopback", "[networth]"){
    sf::UdpSocket socket;
    unsigned short port = 50000;
    sf::Packet send_pkt, recv_pkt;

    // Create values, pack sending packet
    int a = 5, w = 0;
    std::string b = "foo", d = "bar", x, z;
    double c = 3.14159265, y;
    send_pkt << a << b << c << d;

    // I left most receive buffers uninitialized, this is fine
    // Just want to make sure value of w actually changes later
    CHECK(a != w);

    // Same test as above, with packets this time. Pretty boring.
    CHECK(socket.bind(port) == sf::Socket::Status::Done);
    std::optional<sf::IpAddress> ip = sf::IpAddress::getLocalAddress();
    CHECK(ip);
    CHECK(socket.send(send_pkt, ip.value(), port) == sf::Socket::Status::Done);
    CHECK(socket.receive(recv_pkt, ip, port) == sf::Socket::Status::Done);

    // Unpack receiving packet, compare
    recv_pkt >> w >> x >> y >> z;
    CHECK(a == w);
    CHECK(b == x);
    CHECK(c == y);
    CHECK(d == z);
}
