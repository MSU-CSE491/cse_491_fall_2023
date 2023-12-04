/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once

#include "Interfaces/NetWorth/NetworkInterface.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Interfaces/MainInterface.hpp"

namespace netWorth{
    /**
     * The interface of our client that will be interacting and connection with our server
     */
    using namespace sf;

    class ClientInterface : public NetworkingInterface, i_2D::MainInterface {
        private:

        protected:

        public:
            /**
             * Default constructor (AgentBase)
             * @param id agent ID
             * @param name agent name
             */
            ClientInterface(size_t id, const std::string & name) : cse491::InterfaceBase(id, name),
                                                                   NetworkingInterface(id, name),
                                                                   i_2D::MainInterface(id, name) {}

            /**
             * Establish connection with server, initializing interface
             * @return True if successful, false if error
             */
            bool Initialize() override {
                // resolve port and IP from entity properties
                m_ip = sf::IpAddress::resolve(NetworkingInterface::GetProperty<std::string>("ip"));
                m_port = NetworkingInterface::GetProperty<unsigned short>("port");

                Packet send_pkt, recv_pkt;
                std::string str;

                // send request message
                send_pkt << "New client requesting connection.";
                if (!SendPacket(send_pkt, m_ip.value(), m_port)) return false;

                // receive from server
                if (!ReceivePacket(recv_pkt, m_ip, m_port)) return false;

                // print received string (Connection established.)
                recv_pkt >> str;
                std::cout << str << std::endl;

                // request map to start send/receive loop
                send_pkt.clear();
                send_pkt << "Requesting map";
                if (!SendPacket(send_pkt, m_ip.value(), m_port)) return false;

                return true;
            }

            /**
             * Choose action for player agent
             * @param grid the client-side grid
             * @param type_options different cell types of the world
             * @param item_map the items that may be apart of the grid
             * @param agent_map the agents that may be apart of the grid
             * @return action ID of the interface
             */
            size_t SelectAction(const cse491::WorldGrid & grid,
                                const cse491::type_options_t & type_options,
                                const cse491::item_map_t & item_set,
                                const cse491::agent_map_t & agent_set) override
            {
                // Receive and draw map
                sf::Packet send_pkt, recv_pkt;
                std::string map;

                ReceivePacket(recv_pkt, m_ip, m_port);
                ProcessPacket(recv_pkt);

                // grab action ID from MainInterface
              std::uint32_t action_id = i_2D::MainInterface::SelectAction(grid, type_options,
                                                                     item_set, agent_set);
                std::cout << action_id << std::endl;

                // Send instruction to server
                send_pkt << action_id;
                SendPacket(send_pkt, m_ip.value(), m_port);

                // Do the action!
                return action_id;
            }

            /**
             * Process packet from server (just print map for now)
             * @param packet packet from server
             */
            void ProcessPacket(Packet packet) override {
                std::string str;
                packet >> str;
                std::cout << str;
            }

    }; // End of ClientInterface
} // End of namespace NetWorth
