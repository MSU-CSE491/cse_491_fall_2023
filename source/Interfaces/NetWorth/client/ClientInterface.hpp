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
        class ClientInterface : public NetworkingInterface {
        private:
            std::unique_ptr<TrashInterface> mTrash;     /// Display interface

            sf::IpAddress mIp;      /// Destination IP address
            sf::UdpSocket mSocket;  /// UDP socket for sending and receiving
            unsigned short mPort;   /// Destination port

        protected:


        public:
            /**
             * ClientInterface constructor (NetworkingInterface superclass)
             * @param id Interface identifier
             * @param name Interface name
             * @param ip_string String for destination IP address, make into IpAddress object
             * @param port Destination port number
             */
            ClientInterface(size_t id, const std::string & name,
                            const std::string & ip_string,
                            unsigned short port) : NetworkingInterface(id, name) {
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
                std::string message = "Ping!";
                if (mSocket.send(message.c_str(), message.size() + 1, mIp, mPort) != Socket::Status::Done) {
                    std::cout << "Could not connect to " << mIp << " at port " << mPort << std::endl;
                    return false;
                }

                // receive pong from server
                size_t received;
                char data[100]; // TODO: Make packets instead of C strings
                if (mSocket.receive(data, 100, received, mIp, mPort) != sf::Socket::Done)
                {
                    std::cout << "Failure to receive" << std::endl;
                    return false;
                }
                std::cout << data << std::endl;

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

                // receive initial map
                if (mSocket.receive(recv_pkt, mIp, mPort) != sf::Socket::Done)
                {
                    std::cout << "Failure to receive" << std::endl;
                    return;
                }

                recv_pkt >> recv_str;
                std::cout << recv_str;

                while (input != 'q')
                {
                    do {
                        std::cin >> input;
                    } while (!std::cin && wait_for_input);

                    switch (input) {
                        case 'w': case 'W': valid_input = true;   break;
                        case 'a': case 'A': valid_input = true;   break;
                        case 's': case 'S': valid_input = true;   break;
                        case 'd': case 'D': valid_input = true;   break;
                        case 'q': case 'Q': valid_input = true;   break;
                        default: valid_input = false;
                    }

                    // If we waited for input, but don't understand it, notify the user.
                    if (wait_for_input && !valid_input) {
                        std::cout << "Unknown key '" << input << "'." << std::endl;
                    } else {
                        if (mSocket.send(&input, 1, mIp, mPort) != Socket::Status::Done) {
                            std::cout << "Could not connect to " << mIp << " at port " << mPort << std::endl;
                            return;
                        }

                        if (mSocket.receive(recv_pkt, mIp, mPort) != sf::Socket::Done)
                        {
                            std::cout << "Failure to receive" << std::endl;
                            return;
                        }

                        recv_pkt >> recv_str;
                        std::cout << recv_str;
                    }

                    valid_input = false;
                }

            }
        }; //End of ClientInterface
    }// End of namespace NetWorth


} // End of namespace cse491