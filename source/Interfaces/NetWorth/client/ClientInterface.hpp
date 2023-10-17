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

namespace netWorth{
/**
 * The interface of our client that will be interacting and connection with our server
 */
    using namespace sf;

    class ClientInterface : public NetworkingInterface {
    private:
        std::optional<IpAddress> m_dest_ip; /// the destination address (server address)
        unsigned short m_dest_port;         /// the destination port (server port)

        /**
         * Get user input to be sent to server
         * @return string corresponding to action
         */
        static std::string GameLoop_GetInput() {
            bool valid_input = false;
            std::string action;
            char input;

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

            return action;
        }

    protected:

    public:
        /**
         * ClientInterface constructor (NetworkingInterface superclass)
         * @param ip_string String for destination IP address, make into IpAddress object
         * @param port Destination port number
         */
        ClientInterface(const std::string & ip_string,
                        unsigned short port) {
            m_dest_ip = IpAddress::resolve(ip_string);
            m_dest_port = port;
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
            Packet send_pkt, recv_pkt;

            // send request message
            send_pkt << "New client requesting connection.";
            if (!SendPacket(send_pkt, m_dest_ip.value(), m_dest_port)) return false;

            // receive from server
            if (!ReceivePacket(recv_pkt, m_dest_ip, m_dest_port)) return false;

            // print received string (Connection established.)
            std::string str;
            recv_pkt >> str;
            std::cout << str << std::endl;

            return true;
        }

        /**
         * Game loop
         */
        void GameLoop() {
            Packet send_pkt, recv_pkt;
            std::string recv_str;

            cse491::WorldGrid grid;
            cse491::type_options_t type_options;
            cse491::item_set_t item_set;
            cse491::agent_set_t agent_set;
            std::string action;

            send_pkt << "Game started.";

            // ask for map
            if (!SendPacket(send_pkt, m_dest_ip.value(), m_dest_port)) return;

            while (action != "quit") {
                // receive map
                if (!ReceivePacket(recv_pkt, m_dest_ip, m_dest_port)) return;

                // print map
                recv_pkt >> recv_str;
                std::cout << std::endl << recv_str;

                // get user input
                action = GameLoop_GetInput();

                // TODO: Unpack recv_pkt into world grid, agent list, etc
                // We need to serialize these classes...
                //recv_pkt >> grid >> type_options >> item_set >> agent_set;

                //action = mTrash->SelectAction(grid, type_options, item_set, agent_set);

                // send packet with action
                send_pkt.clear();
                send_pkt << action;

                if (!SendPacket(send_pkt, m_dest_ip.value(), m_dest_port)) return;
            }
        }
    }; //End of ClientInterface
}// End of namespace NetWorth
