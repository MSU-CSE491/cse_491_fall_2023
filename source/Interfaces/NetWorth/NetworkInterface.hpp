/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>

#include "../../core/InterfaceBase.hpp"

using namespace sf;

namespace cse491 {
    namespace netWorth{
    /**
     * TODO: Delete or incorporate
     */
        class NetworkingInterface{
        private:

        protected:
            UdpSocket mSocket; ///The socket we are going to make our connection
            IpAddress mLocalAddress; ///the local address of the server
            //Thought about making mClients a shared pointer to a vector, but it'll be a vector for now
            std::vector<std::string> mClients; ///list of all the clients that will connect with the server
            unsigned short mPort;

        public:
            NetworkingInterface() = default;
            ~NetworkingInterface() = default;
            /**
             * Receives a socket that has been connected between client and server
             * @return the udp socket
             */
            UdpSocket * GetSocket(){
                return &mSocket;
            }
//            virtual Packet CreateActionPacket(std::string action){
//                //Creating packet to send to server of actions
//                Packet packet;
//                size_t action_num = action_map[action];
//                packet << static_cast<int>(action_num);
//                return packet;
//            }

            /**
             * Sends a packet across the socket
             * @param packet the packet we want to send
             * @param destAddr the destination address we want to send to
             * @param port the port of the connection
             */
            virtual void SendPacket(Packet packet, IpAddress destAddr, const unsigned short port){
                mSocket.send(packet, destAddr, port);
            }
            /**
             * Starts the connection by receiving the first packet
             * @return the IP of the server
             */
            virtual IpAddress ReceivePacket(){
                char buffer[1024];
                std::size_t received = 0;
                sf::IpAddress sender;
                unsigned short port;
                mSocket.receive(buffer, sizeof(buffer), received, sender, port);

                if (received){
                    sf::Packet messagePacket;
                    std::string message = "Pong";
                    messagePacket << message;
                    mSocket.send(messagePacket, sender, port);
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
    }
  

} // End of namespace cse491