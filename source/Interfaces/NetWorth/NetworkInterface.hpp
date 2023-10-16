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
        std::optional<IpAddress> m_ip; /// the local address of the machine
        unsigned short m_port;          /// local port number of the machine
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
         * @return true if successfully sent
         */
        virtual bool SendPacket(Packet packet, IpAddress destAddr, const unsigned short port){
            return m_socket.send(packet, destAddr, port) == Socket::Status::Done;
        }
        /**
         * Starts the connection by receiving the first packet
         * @return the IP of the server
         */
        virtual IpAddress ReceivePacket(){
            Packet packet;
            std::optional<IpAddress> sender;
            unsigned short port;
            if (m_socket.receive(packet, sender, port) != Socket::Status::Done) {
                std::cout << "Failed to receive" << std::endl;
            }

            if (packet){
                Packet messagePacket;
                std::string message = "Pong";
                messagePacket << message;
                SendPacket(messagePacket, sender.value(), port);
            }
            return sender.value();
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
