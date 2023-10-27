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
        unsigned short m_dest_port = 0;         /// the destination port (server port)

    protected:

    public:
        /**
         * Default constructor (AgentBase)
         * @param id agent ID
         * @param name agent name
         */
        ClientInterface(size_t id, const std::string & name) : NetworkingInterface(id, name) {}


        /**
         * Establish connection with server, initializing interface
         * @return True if successful, false if error
         */
        bool Initialize() override{
            m_dest_ip = sf::IpAddress::resolve(GetProperty<std::string>("ip"));
            m_dest_port = GetProperty<unsigned short>("port");

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

            send_pkt.clear();
            send_pkt << "Requesting map";
            if (!SendPacket(send_pkt, m_dest_ip.value(), m_dest_port)) return false;
            return true;
        }

        /**
         * Choose action for player agent
         * @param grid
         * @param type_options
         * @param item_set
         * @param agent_set
         * @return
         */
        size_t SelectAction(const cse491::WorldGrid & grid,
                            const cse491::type_options_t & type_options,
                            const cse491::item_set_t & item_set,
                            const cse491::agent_set_t & agent_set) override
        {

            sf::Packet recv_pkt;
            std::string map;

            // Receive and draw map
            ReceivePacket(recv_pkt, m_dest_ip, m_dest_port);
            recv_pkt >> map;
            std::cout << map;

            // Take input
            char input;
            do {
                std::cin >> input;
            } while (!std::cin);

            // Grab action ID
            size_t action_id = 0;
            std::string action_str;
            switch (input) {
                case 'w': case 'W': action_str = "up";      break;
                case 'a': case 'A': action_str = "left";    break;
                case 's': case 'S': action_str = "down";    break;
                case 'd': case 'D': action_str = "right";   break;
                case 'q': case 'Q': action_str = "quit";    break;
            }
            action_id = GetActionID(action_str);

            // If we waited for input, but don't understand it, notify the user.
            if (action_id == 0 && action_str != "quit") {
                std::cout << "Unknown key '" << input << "'." << std::endl;
            }

            // Send instruction to server
            sf::Packet send_pkt;
            send_pkt << action_str;
            SendPacket(send_pkt, m_dest_ip.value(), m_dest_port);

            if (action_str == "quit") exit(0);

            // Do the action!
            return action_id;
        }

    }; //End of ClientInterface
}// End of namespace NetWorth
