/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once
#include <map>
#include <sstream>
#include <vector>

namespace netWorth{
    using namespace sf;

    /**
     * The server that will be running and that allows clients to connect to
     */
    class ClientManager {
    private:
        sf::UdpSocket *m_socket = nullptr;                /// Socket shared with ClientInterface
        sf::UdpSocket *m_game_update_socket = nullptr;    /// Game update socket (for agent updates)
        std::optional<sf::IpAddress> m_ip;      /// Server IP address
		unsigned short m_update_port = 0; ///Port to update the game
        std::unordered_map<size_t, size_t> m_action_map;     ///Map of agent IDs to most recent action selected
        size_t m_client_id = 0;		///Id of client
    protected:

    public:

        /**
         * Default constructor (AgentBase)
         * @param id agent ID
         * @param name agent name
         */
        ClientManager()= default;

		void setUpdatePort(unsigned short port) {m_update_port = port;}

        /**
         * Turn packet from server into action map for ControlledAgents
         * @param pkt received packet
         */
        void packetToActionMap(sf::Packet pkt) {
            size_t dataSize, agentID, actionID;
            pkt >> dataSize;
            for (size_t i = 0; i < dataSize; i++) {
                pkt >> agentID >> actionID;
                m_action_map[agentID] = actionID;
            }
        }

		/**
		 * Set receiving socket for action map and IP/port info
		 * @param socket pointer to ClientInterface's socket
		 * @param ip server IP
		 * @param port server port
		 */
        void setupSocket(sf::UdpSocket *socket, std::optional<sf::IpAddress> ip) {
            m_socket = socket;
            m_ip = ip;
        }

        /**
         * Set socket for game updates
         * @param socket pointer to socket
         */
        void setupGameUpdateSocket(sf::UdpSocket *socket) {
            m_game_update_socket = socket;
            m_game_update_socket->setBlocking(false);
			if (m_game_update_socket->bind(m_update_port) != Socket::Status::Done){
				std::cerr << "Failed to setup game update socket" << std::endl;
			}
        }

        /**
         * Check if Agent ID is present in agent action map
         * @param id Agent ID
         * @return true if ID is present
         */
        bool iDPresent(size_t id) {
            return m_action_map.contains(id);
        }

        /**
         * Return action ID correspoding to agent ID
         * @param id Agent ID
         * @return action ID
         */
        size_t getActionID(size_t id) {
            return m_action_map[id];
        }

        /**
         * Clear action map after ClientInterface moves
         */
        void clearActionMap() {
            m_action_map.clear();
        }

        /**
         * Receive serialized agent data for midgame updates
         * @return serialized data (or empty if no update)
         */
        std::string getSerializedAgents() {
            sf::Packet recvPkt;
            std::optional<sf::IpAddress> tempIP;
            unsigned short tempPort;
            if (m_game_update_socket->receive(recvPkt, tempIP, tempPort) == sf::Socket::Status::Done) {
                std::string data;
                recvPkt >> data;
                return data;
            }
            return "";
        }


        void setClientID(size_t id) {
            m_client_id = id;
        }

        size_t getClientID() const{
            return m_client_id;
        }

    }; // End of class ClientManager
} // End of namespace netWorth