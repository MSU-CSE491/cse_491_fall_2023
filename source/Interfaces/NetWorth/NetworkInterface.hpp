/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <array>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include "../../core/InterfaceBase.hpp"

namespace netWorth{

    using namespace sf;
/**
 * TODO: Delete or incorporate
 */
    class NetworkingInterface {
    private:

    protected:
        UdpSocket m_socket; ///The socket we are going to make our connection
        IpAddress m_local_address; ///the local address of the server
        //Thought about making m_clients a shared pointer to a vector, but it'll be a vector for now
        std::vector<std::string> m_clients; ///list of all the clients that will connect with the server

    public:
        NetworkingInterface() = default;
        ~NetworkingInterface() = default;
        /**
         * Receives a socket that has been connected between client and server
         * @return the udp socket
         */
        UdpSocket * GetSocket(){
            return &m_socket;
        }

        /**
         * Sends a packet across the socket
         * @param packet the packet we want to send
         * @param destAddr the destination address we want to send to
         * @param port the port of the connection
         */
        virtual void SendPacket(Packet packet, IpAddress destAddr, const unsigned short port){
            m_socket.send(packet, destAddr, port);
        }
        /**
         * Starts the connection by receiving the first packet
         * @return the IP of the server
         */
        virtual IpAddress ReceivePacket(){
            std::array<char, 1024> buffer{};
            std::size_t received = 0;
            sf::IpAddress sender;
            unsigned short port;
            m_socket.receive(buffer.data(), sizeof(buffer), received, sender, port);

            if (received){
                sf::Packet messagePacket;
                std::string message = "Pong";
                messagePacket << message;
                m_socket.send(messagePacket, sender, port);
            }
            return sender;
        }
        /**
         * Processes the packet and outputs it
         * @param packet the packet we want to output
         */
        virtual void ProcessPacket(Packet packet){
            std::string actionInd;
            packet >> actionInd;
            std::cout << actionInd;
        }
    };
}//End of namespace networth
