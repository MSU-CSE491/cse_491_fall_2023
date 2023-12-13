/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a network
 * @note Status: PROTOTYPE
 **/

#pragma once

#include <cassert>
#include <string>
#include <thread>
#include <vector>
#include <array>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include "../../core/InterfaceBase.hpp"

namespace netWorth{

    using namespace sf;

    class NetworkingInterface : public virtual cse491::InterfaceBase {
        private:

        protected:
            UdpSocket m_socket;             ///The socket we are going to make our connection
            std::optional<IpAddress> m_ip;  /// the destination IP of the machine this communicates with
            unsigned short m_port = 0;      /// the destination port of the machine this communicates with

        public:
            /**
             * Default constructor (AgentBase)
             * @param id agent ID
             * @param name agent name
             */
            NetworkingInterface(size_t id, const std::string & name) : cse491::InterfaceBase(id, name) {

            }

		/**
		 * Receives a socket that has been connected between client and server
		 * @return the udp socket
		 */
            virtual UdpSocket * getSocket(){
                return &m_socket;
            }

            /**
             * Bind socket to port number
             * @param socket Socket to be bound
             * @param port Port number
             * @return true if successful
             */
            virtual bool bindSocket(UdpSocket &socket, unsigned short port) {
                std::cout << "Binding socket on port: " << port << std::endl;
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
            virtual bool sendPacket(Packet packet, IpAddress destAddr, const unsigned short port){
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
            virtual bool receivePacket(Packet & pkt, std::optional<IpAddress> &sender, unsigned short &port){
                if (m_socket.receive(pkt, sender,port) != Socket::Status::Done) {
                    std::cerr << "Failed to receive" << std::endl;
                    return false;
                }
                return true;
            }

        }; // End of NetworkingInterface
} // End of namespace netWorth
