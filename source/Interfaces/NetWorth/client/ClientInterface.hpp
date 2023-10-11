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
#include <memory>

#include "../NetworkInterface.hpp"
#include "../../TrashInterface.hpp"

using namespace sf;

namespace cse491 {
    namespace netWorth{
    /**
     * The interface of our client that will be interacting and connection with our server
     */
        class ClientInterface {
        private:
            std::unique_ptr<TrashInterface> mTrash;     /// Display interface

            sf::IpAddress mIp;      /// Destination IP address
            sf::UdpSocket mSocket;  /// UDP socket for sending and receiving
            unsigned short mPort;   /// Destination port

        protected:


        public:
            /**
             * ClientInterface constructor (NetworkingInterface superclass)
             * @param ip_string String for destination IP address, make into IpAddress object
             * @param port Destination port number
             */
            ClientInterface(const std::string & ip_string,
                            unsigned short port) {
                mIp = sf::IpAddress(ip_string);
                mPort = port;
            }

            /**
             * Default destructor
             */
            ~ClientInterface() = default;

            /**
             * Establish connection with server
             * @return True if successful, false if error
             */
            bool EstablishConnection() {
                // send ping to server
                sf::Packet send_pkt;
                sf::Packet recv_pkt;
                std::string str = "Ping!";
                send_pkt << str;

                if (mSocket.send(send_pkt, mIp, mPort) != Socket::Status::Done) {
                    std::cout << "Could not connect to " << mIp << " at port " << mPort << std::endl;
                    return false;
                }

                // receive pong from server
                if (mSocket.receive(recv_pkt, mIp, mPort) != sf::Socket::Done)
                {
                    std::cout << "Failure to receive" << std::endl;
                    return false;
                }
                recv_pkt >> str;
                std::cout << str << std::endl;

                return true;
            }

            /**
             * Game loop
             */
            void Loop() {
                char input;
                bool valid_input = false;
                bool wait_for_input = true;

                sf::Packet recv_pkt;
                std::string recv_str;
                sf::Packet send_pkt;

                recv_pkt >> recv_str;
                std::cout << recv_str;

                WorldGrid grid;
                type_options_t type_options;
                item_set_t item_set;
                agent_set_t agent_set;
                std::string action;

                std::string send_str = "Game started";
                send_pkt << send_str;

                // ask for map
                if (mSocket.send(send_pkt, mIp, mPort) != Socket::Status::Done) {
                    std::cout << "Could not connect to " << mIp << " at port " << mPort << std::endl;
                    return;
                }

                while (action != "quit")
                {
                    // receive initial map
                    if (mSocket.receive(recv_pkt, mIp, mPort) != sf::Socket::Done)
                    {
                        std::cout << "Failure to receive" << std::endl;
                        return;
                    }
                    recv_pkt >> recv_str;
                    std::cout << std::endl << recv_str;

                    valid_input = false;
                    while (!valid_input) {
                        do {
                            std::cin >> input;
                        } while (!std::cin);

                        switch (input) {
                            case 'w': case 'W': action = "up";      valid_input = true; break;
                            case 'a': case 'A': action = "left";    valid_input = true; break;
                            case 's': case 'S': action = "down";    valid_input = true; break;
                            case 'd': case 'D': action = "right";   valid_input = true; break;
                            case 'q': case 'Q': action = "quit";    valid_input = true; break;
                            default: valid_input = false;
                        }
                        if (!valid_input) {
                            std::cout << "Your move?";
                        }
                    }

                    // TODO: Unpack recv_pkt into world grid, agent list, etc
                    // We need to serialize these classes...
                    //recv_pkt >> grid >> type_options >> item_set >> agent_set;

                    //action = mTrash->SelectAction(grid, type_options, item_set, agent_set);
                    //recv_str = "ping!";
                    send_pkt.clear();
                    send_pkt << action;

                    if (mSocket.send(send_pkt, mIp, mPort) != Socket::Status::Done) {
                        std::cout << "Could not connect to " << mIp << " at port " << mPort << std::endl;
                        return;
                    }
                }
            }
        }; //End of ClientInterface
    }// End of namespace NetWorth


} // End of namespace cse491