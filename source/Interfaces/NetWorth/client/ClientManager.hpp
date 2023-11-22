/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once
#include <map>
#include <sstream>
#include <vector>
#include "Interfaces/NetWorth/NetworkInterface.hpp"

namespace netWorth{
    using namespace sf;

    /**
     * The server that will be running and that allows clients to connect to
     */
    class ClientManager {
    private:

    protected:

    public:
        std::map<size_t, size_t> m_action_map; ///Map of agent IDs to most recent action selected

        /**
         * Default constructor (AgentBase)
         * @param id agent ID
         * @param name agent name
         */
        ClientManager()= default;

        /**
         * Turn packet from server into action map for ControlledAgents
         * @param pkt received packet
         */
        void PacketToActionMap(sf::Packet pkt) {
            size_t data_size, agent_id, action_id;
            pkt >> data_size;
            for (size_t i = 0; i < data_size; i++) {
                pkt >> agent_id >> action_id;
                m_action_map[agent_id] = action_id;
            }
        }

    }; // End of class ClientManager
} // End of namespace netWorth