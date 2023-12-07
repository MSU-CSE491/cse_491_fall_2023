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
                m_manager->SetupSocket(&m_socket, m_ip);
                m_manager->SetClientID(id);
                std::cout << "Interface created with ID " << id << std::endl;

                Packet sendPkt, recvPkt;
                setMInputWaitTime(0.25f);

                // send request message
                sendPkt << "New client requesting connection.";
                std::cout << "Requesting connection" << std::endl;
                auto ip = m_ip.value();
                if (!SendPacket(sendPkt, ip, m_port)) return false;

                // receive from server
                if (!ReceivePacket(recvPkt, m_ip, m_port)) return false;

                // print received string (Connection established.)
                std::string msg;
				recvPkt >> msg;
                std::cout << msg << std::endl;

                // request map to start send/receive loop
				sendPkt.clear();
				sendPkt << "Requesting start";
                if (!SendPacket(sendPkt, m_ip.value(), m_port)) return false;

                // receive action map from server for previous agents
                ReceivePacket(recvPkt, m_ip, m_port);
                m_manager->PacketToActionMap(recvPkt);

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
                sf::Packet sendPkt, recvPkt;

                // grab action ID from MainInterface
                size_t action_id = i_2D::MainInterface::SelectAction(grid, type_options,
                            item_set, agent_set);
                std::cout << action_id << std::endl;

                // Send instruction to server
                sendPkt << action_id;
                SendPacket(sendPkt, m_ip.value(), m_port);

                m_manager->ClearActionMap();
                DrawGrid(grid, type_options, item_set, agent_set);

                // await action map from server
                ReceivePacket(recvPkt, m_ip, m_port);
                m_manager->PacketToActionMap(recvPkt);

                // Do the action!
                return action_id;
            }

    }; // End of ClientInterface
} // End of namespace NetWorth
