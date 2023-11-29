/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once

#include "Interfaces/NetWorth/NetworkInterface.hpp"
#include "Interfaces/TrashInterface.hpp"
#include "Interfaces/MainInterface.hpp"
#include "ClientManager.hpp"

namespace netWorth{
    /**
     * The interface of our client that will be interacting and connection with our server
     */
    using namespace sf;

    class ClientInterface : public NetworkingInterface, i_2D::MainInterface {
        private:
            unsigned short m_serverPort;
            netWorth::ClientManager *m_manager = nullptr;

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
                m_ip = sf::IpAddress::resolve(NetworkingInterface::GetProperty<std::string>("server_ip"));
                m_port = NetworkingInterface::GetProperty<unsigned short>("server_port");
                m_manager = GetProperty<netWorth::ClientManager *>("manager");
                m_manager->SetupSocket(&m_socket, m_ip, m_port);

                Packet send_pkt, recv_pkt,two_pkt;

                // send request message
                send_pkt << "New client requesting connection.";
                std::cout << "Requesting connection" << std::endl;
                auto ip = m_ip.value();
                if (!SendPacket(send_pkt, ip, m_port)) return false;

                // receive from server
                if (!ReceivePacket(recv_pkt, m_ip, m_port)) return false;
                std::cout<<"lol" << std::endl;
                // print received string (Connection established.)
                recv_pkt >> m_serverPort;
                std::cout << m_serverPort << std::endl;

                // request map to start send/receive loop
                send_pkt.clear();
                send_pkt << "Requesting map";
                if (!SendPacket(send_pkt, m_ip.value(), m_serverPort)) return false;

                std::cout<<"I love bread" << std::endl;
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
                sf::Packet send_pkt, rec_pkt;
                std::string map;
                std::cout<<"going to receive packet" << std::endl;
                ReceivePacket(rec_pkt, m_ip, m_serverPort);
                rec_pkt >> map;
                std::cout<<"hiiiiii" << std::endl;
                ProcessPacket(rec_pkt);

                // grab action ID from MainInterface
                size_t action_id = i_2D::MainInterface::SelectAction(grid, type_options,
                            item_set, agent_set);
                std::cout << action_id << std::endl;


                // Send instruction to server
                send_pkt << action_id;
                SendPacket(send_pkt, m_ip.value(), m_port);

                m_manager->ClearActionMap();

                std::cout<<"end";
                // Do the action!
                return action_id;
            }

            /**
             * Process packet from server (just print agent action map for now)
             * @param packet packet from server
             */
            void ProcessPacket(Packet packet) override {
                size_t data_size, data;
                packet >> data_size;
                std::cout << data_size << " agents" << std::endl;
                for (size_t i = 0; i < data_size; i++) {
                    packet >> data;
                    std::cout << "agent " << data;
                    packet >> data;
                    std::cout << " action " << data << std::endl;
                }
                std::cout << std::endl;
            }

    }; // End of ClientInterface
} // End of namespace NetWorth
