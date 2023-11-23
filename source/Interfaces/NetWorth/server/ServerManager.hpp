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
    class ServerManager {
    private:

    protected:

    public:
        std::map<size_t, size_t> m_action_map; ///Map of agent IDs to most recent action selected

        const static constexpr unsigned short m_initConnectionPort = 55000; ///Port for initial client connection

        unsigned short m_maxClientPort = 55000; ///Port that is incremented for client thread handoff

        /**
         * Default constructor (AgentBase)
         * @param id agent ID
         * @param name agent name
         */
        ServerManager()= default;

        /**
         * Report action from agent to manager
         * @param entity_id ID of reorting entity
         * @param action_id action ID
         */
        void TellAction(size_t entity_id, size_t action_id) {
            m_action_map[entity_id] = action_id;
//            for (auto pair : m_action_map) {
//                std::cout << pair.first << ' ' << pair.second << std::endl;
//            }
        }

        /**
         * Convert action map to packet to send to client
         * @return packet containing action map as series of integers
         */
        sf::Packet ActionMapToPacket() {
            sf::Packet pkt;
            pkt << m_action_map.size();
            for (auto pair : m_action_map) {
                pkt << pair.first << pair.second;
            }
            //std::cout << m_action_map.size();
            return pkt;
        }

    }; // End of class ServerManager
} // End of namespace netWorth