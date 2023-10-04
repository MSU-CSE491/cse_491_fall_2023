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

#include "../NetworkInterface.hpp"
#include "networkingworld.hpp"

using namespace sf;

namespace cse491 {
    namespace netWorth{
        class ServerInterface : public NetworkingInterface {
        private:
        std::shared_ptr<cse491::netWorth::NetworkMazeWorld> mWorld;


        protected:


        public:
            ServerInterface() : NetworkingInterface() { }
            ~ServerInterface() = default;


            void ProcessPacket(sf::Packet packet) override{

            }
        };//End of class ServerInterface
    }//End of namespace netWorth
} // End of namespace cse491