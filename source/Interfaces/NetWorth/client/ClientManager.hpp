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
        sf::UdpSocket *m_socket;                /// Socket shared with ClientInterface
        std::optional<sf::IpAddress> m_ip;      /// Server IP address
        unsigned short m_port;                  /// Server port
        std::unordered_map<size_t, size_t> m_action_map;     ///Map of agent IDs to most recent action selected

    protected:

    public:
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
            uint64_t data_size, agent_id, action_id;
            pkt >> data_size;
            for (size_t i = 0; i < data_size; i++) {
                pkt >> agent_id >> action_id;
                m_action_map[agent_id] = action_id;
            }
        }

        /**
         * Set receiving socket for action map and IP/port info
         * @param socket pointer to ClientInterface's socket
         * @param ip server IP
         * @param port server port
         */
        void SetupSocket(sf::UdpSocket *socket, std::optional<sf::IpAddress> ip, unsigned short port) {
            m_socket = socket;
            m_ip = ip;
            m_port = port;
        }

        /**
         * Wait until server sends action map
         */
        void RequestActionMap() {
            sf::Packet recv_pkt;
            if (m_socket->receive(recv_pkt, m_ip, m_port) != sf::Socket::Status::Done) {
                std::cerr << "Failed to receive" << std::endl;
                return;
            }
            PacketToActionMap(recv_pkt);
        }

        /**
         * Check if Agent ID is present in agent action map
         * @param id Agent ID
         * @return true if ID is present
         */
        bool IdPresent(size_t id) {
            return m_action_map.find(id) == m_action_map.end();
        }

        /**
         * Return action ID correspoding to agent ID
         * @param id Agent ID
         * @return action ID
         */
        size_t GetActionID(size_t id) {
            return m_action_map[id];
        }

        /**
         * Clear action map after ClientInterface moves
         */
        void ClearActionMap() {
            m_action_map.clear();
        }

    }; // End of class ClientManager
} // End of namespace netWorth