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
        class NetworkingInterface : public InterfaceBase {
        private:
            UdpSocket mSocket;
            std::string mServerIPAddress = "35.12.211.81";
            //Thought about making mClients a shared pointer to a vector, but it'll be a vector for now
            std::vector<std::string> mClients;
            const unsigned short mClientPort = 5100;
            const unsigned short mServerPort = 5101;

        protected:
            

        public:
            NetworkingInterface(size_t id, const std::string & name) : InterfaceBase(id, name) { }
            ~NetworkingInterface() = default;


            Packet CreateAPacket(std::string action){
                //Creating packet to send to server of actions
                Packet packet;
                size_t action_num = action_map[action];
                packet << static_cast<int>(action_num);
                return packet;
            }


            void SendClientPacket(Packet packet, IpAddress destAddr){
                mSocket.send(packet, destAddr, mServerPort);
            }

            void SendServerPacket(Packet packet, IpAddress destAddr){
                mSocket.send(packet, destAddr, mClientPort);
            }

            void ReceivePacket(){
                char buffer[1024];
                std::size_t received = 0;
                sf::IpAddress sender;
                unsigned short port;
                mSocket.receive(buffer, sizeof(buffer), received, sender, port);
                std::cout << "Packet receieved. IP address: " << sender << std::endl;
            }


        };
    }
  

} // End of namespace cse491