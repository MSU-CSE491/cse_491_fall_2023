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
        virtual ~NetworkingInterface() = default;

        /**
         * Receives a socket that has been connected between client and server
         * @return the udp socket
         */
        UdpSocket * GetSocket(){
            return &m_socket;
        }

        /**
         * Bind socket to port number
         * @param socket Socket to be bound
         * @param port Port number
         * @return true if successful
         */
        virtual bool BindSocket(UdpSocket &socket, unsigned short port) {
            if (socket.bind(port) != Socket::Status::Done) {
                std::cerr << "Failed to bind socket" << std::endl;
                return false;
            }
            return true;
        }

        /**
         * Sends a packet across the socket
         * @param packet the packet we want to send
         * @param destAddr the destination address we want to send to
         * @param port the port of the connection
         * @return true if successfully sent
         */
        virtual bool SendPacket(Packet packet, IpAddress destAddr, const unsigned short port){
            if (m_socket.send(packet, destAddr, port) != Socket::Status::Done) {
                std::cerr << "Could not connect to" << destAddr << " at port " << port << std::endl;
                return false;
            }
            return true;
        }
        /**
         * Starts the connection by receiving the first packet
         * @param sender IP of sending machine
         * @param port port number of sending machine
         * @return received packet
         */
        virtual bool ReceivePacket(Packet & pkt, std::optional<IpAddress> &sender, unsigned short &port){
            if (m_socket.receive(pkt, sender, port) != Socket::Status::Done) {
                std::cerr << "Failed to receive" << std::endl;
                return false;
            }
            return true;
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
