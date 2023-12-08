/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A networking interface that allows information to be sent across a  network
 * @note Status: PROTOTYPE
 **/

#pragma once
#include <thread>
#include <map>
#include <sstream>
#include <utility>
#include <vector>
#include "Interfaces/NetWorth/NetworkInterface.hpp"

namespace netWorth{
    using namespace sf;

    /**
     * The server that will be running and that allows clients to connect to
     */
    class ServerManager {
    private:

		sf::UdpSocket m_manager_socket;

        std::set<size_t> m_interface_set;   /// Set of interfaces on the server

        std::map<size_t, size_t> m_action_map; ///Map of agent IDs to most recent action selected

        std::string m_current_serialized_agents; ///String with all current serialized agents

		std::vector<std::pair<sf::IpAddress, unsigned short>> m_update_vec; ///IP Addresses and ports to send updates to

		bool m_has_new_agent = false; ///Boolean that states if this manager has gotten a new agent

		bool m_interfaces_present = false; ///Boolean that states if there are interfaces present on the server

    protected:

    public:
        const static constexpr unsigned short m_init_connection_port = 55000; ///Port for initial client connection

        unsigned short m_max_client_port = 55000; ///Port that is incremented for client thread handoff

		/**
		 * Default constructor (AgentBase)
		 * @param id agent ID
		 * @param name agent name
		 */
        ServerManager() = default;

		/**
		 * Returns the current serialized agents
		 * @return a string of the current serialized agents
		 */
        std::string getSerializedAgents(){return m_current_serialized_agents;}

		/**
		 * Sets the current serialized agents of the server
		 * @param serializedAgents string reference representing the serialized agents
		 */
        void setSerializedAgents(std::string & serializedAgents) { m_current_serialized_agents = serializedAgents;}

		/**
		 * Returns if there are agents present on the server as a boolean
		 * @return boolean representing if there are agents present on the server
		 */
		[[nodiscard]] bool hasAgentsPresent() const {return m_interfaces_present;}

		/**
		 * Sets a boolean stating if the server has received a new agent
		 * @param hasNewAgent boolean stating that a new agent has joined
		 */
		void setNewAgent(bool hasNewAgent){ m_has_new_agent = hasNewAgent;}

        /**
         * Convert action map to packet to send to client
         * @return packet containing action map as series of integers
         */
        sf::Packet actionMapToPacket()
        {
            sf::Packet pkt;

            // serialize action map
            pkt << m_action_map.size();
            for (auto pair: m_action_map) {
                pkt << pair.first << pair.second;
            }

            return pkt;
        }

        /**
         * Increases the max client port
         */
        void increasePort(){++m_max_client_port;}

		/**
		 * Removes an interface by ID
		 * @param id
		 */
        void removeInterface(size_t id){
            m_interface_set.erase(id);
            if (m_interface_set.empty()) m_interfaces_present = false;
        }

		/**
		 * Adds an IP and port to a vector for group update
		 * @param ip IP address of client receiving updates
		 * @param port port of client receiving updates
		 */
		void addToUpdatePairs(sf::IpAddress ip, unsigned short port){
			m_update_vec.emplace_back(ip, port);
		}

		/**
		 * Sends game updates to all clients when a new agent joins
		 */
		 void sendGameUpdates(){
			if (m_has_new_agent)
			{
				sf::Packet serializedAgentPkt;
				serializedAgentPkt << getSerializedAgents();
				//Loops through all pairs of IP and port and sends serialized agents
				for (auto client: m_update_vec){
					std::cout << "sending game updates to IP: " << client.first.toString() << " with port " <<
					client.second << std::endl;
					if (m_manager_socket.send(serializedAgentPkt, client.first, client.second) != sf::Socket::Status::Done) {
                        std::cerr << "Error sending updates to client at " << client.first.toString() << " port " << client.second << std::endl;
                    }
				}
				m_has_new_agent = false;
			}
		}

		/**
		 * Removes an interface from action map by key
		 * @param key
		 */
        void removeFromActionMap(size_t key){
            m_action_map.erase(key);
        }

		/**
		 * Removes ip and port from the update vector
		 * @param ip ip to remove
		 * @param port port to remove
		 */
		void removeFromUpdatePairs(sf::IpAddress ip, unsigned short port){
			m_update_vec.erase(std::remove_if(m_update_vec.begin(), m_update_vec.end(),
				[ip, port](std::pair<sf::IpAddress, unsigned short> pair){
				return (pair.first == ip && pair.second == port);
			}), m_update_vec.end());
		}

		/**
		 * Writes to the action map
		 * @param key to reference
		 * @param val to write
		 */
        void writeToActionMap(size_t key, size_t val){
            m_action_map.insert_or_assign(key, val);
        }

		/**
		 * Adds and interface to the interface set
		 * @param agent_id
		 */
        void addToInterfaceSet(size_t agent_id){
            m_interface_set.insert(agent_id);
			m_interfaces_present = true;
        }

    }; // End of class ServerManager
} // End of namespace netWorth