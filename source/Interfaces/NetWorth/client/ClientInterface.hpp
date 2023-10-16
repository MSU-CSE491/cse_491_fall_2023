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

//#include "../NetworkInterface.hpp"
#include "../../TrashInterface.hpp"

namespace netWorth{
/**
 * The interface of our client that will be interacting and connection with our server
 */
    using namespace sf;
    class ClientInterface {
    private:

        std::optional<IpAddress> m_ip;      /// Destination IP address
        UdpSocket m_socket;  /// UDP socket for sending and receiving
        unsigned short m_port;   /// Destination port

    protected:


    public:
        /**
         * ClientInterface constructor (NetworkingInterface superclass)
         * @param ip_string String for destination IP address, make into IpAddress object
         * @param port Destination port number
         */
        ClientInterface(const std::string & ip_string,
                        unsigned short port) {
            m_ip = IpAddress::resolve(ip_string);
            m_port = port;
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
            Packet send_pkt;
            Packet recv_pkt;
            std::string str = "Ping!";
            send_pkt << str;

            if (m_socket.send(send_pkt, m_ip.value(), m_port) != Socket::Status::Done) {
                std::cout << "Could not connect to " << m_ip.value() << " at port " << m_port << std::endl;
                return false;
            }

            // receive pong from server
            if (m_socket.receive(recv_pkt, m_ip, m_port) != Socket::Status::Done)
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
        void GameLoop() {
            char input;
            bool valid_input = false;

            Packet recv_pkt;
            std::string recv_str;
            Packet send_pkt;

            recv_pkt >> recv_str;
            std::cout << recv_str;

            cse491::WorldGrid grid;
            cse491::type_options_t type_options;
            cse491::item_set_t item_set;
            cse491::agent_set_t agent_set;
            std::string action;

            std::string send_str = "Game started";
            send_pkt << send_str;

            // ask for map
            if (m_socket.send(send_pkt, m_ip.value(), m_port) != Socket::Status::Done) {
                std::cout << "Could not connect to " << m_ip.value() << " at port " << m_port << std::endl;
                return;
            }

            while (action != "quit")
            {
                // receive initial map
                if (m_socket.receive(recv_pkt, m_ip, m_port) != Socket::Status::Done)
                {
                    std::cout << "Failure to receive" << std::endl;
                    return;
                }
                recv_pkt >> recv_str;
                std::cout << std::endl << recv_str;

                valid_input = false;
                while (!valid_input) {
                    std::cin >> input;
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

                if (m_socket.send(send_pkt, m_ip.value(), m_port) != Socket::Status::Done) {
                    std::cout << "Could not connect to " << m_ip.value() << " at port " << m_port << std::endl;
                    return;
                }
            }
        }
    }; //End of ClientInterface
}// End of namespace NetWorth
