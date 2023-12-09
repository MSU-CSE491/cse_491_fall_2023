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
				m_manager->setupSocket(&m_socket, m_ip);
				m_manager->setClientID(id);

                Packet sendPkt, recvPkt,twoPkt;
                setMInputWaitTime(0.25f);

                // send request message
                sendPkt << "New client requesting connection.";
                auto ip = m_ip.value();
                if (!sendPacket(sendPkt, ip, m_port)) return false;

                // receive from server
                if (!receivePacket(recvPkt, m_ip, m_port)) return false;
                // print received string (Connection established.)
                std::string msg;
                recvPkt >> msg;
                std::cout << msg << std::endl;

                // request map to start send/receive loop
                sendPkt.clear();
                sendPkt << "Requesting start";
                if (!sendPacket(sendPkt, m_ip.value(), m_port)) return false;


                // receive action map from server for previous agents
				receivePacket(recvPkt, m_ip, m_port);
				m_manager->packetToActionMap(recvPkt);

                return true;
            }

            /**
             * Choose action for player agent
             * @param grid the client-side grid
             * @param typeOptions different cell types of the world
             * @param item_map the items that may be apart of the grid
             * @param agent_map the agents that may be apart of the grid
             * @return action ID of the interface
             */
            size_t SelectAction(const cse491::WorldGrid & grid,
                                const cse491::type_options_t & typeOptions,
                                const cse491::item_map_t & itemMap,
                                const cse491::agent_map_t & agentMap) override
            {
                // Receive and draw map
                sf::Packet sendPkt, recvPkt;

                // grab action ID from MainInterface
                size_t actionID = i_2D::MainInterface::SelectAction(grid, typeOptions,
                            itemMap, agentMap);

                // Send instruction to server
                sendPkt << actionID;
				sendPacket(sendPkt, m_ip.value(), m_port);

				m_manager->clearActionMap();
                DrawGrid(grid, typeOptions, itemMap, agentMap);

                // await action map from server
				receivePacket(recvPkt, m_ip, m_port);
				m_manager->packetToActionMap(recvPkt);

                // Do the action!
                return actionID;
            }

    }; // End of ClientInterface
} // End of namespace NetWorth
