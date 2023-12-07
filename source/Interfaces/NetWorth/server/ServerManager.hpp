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
        //std::map<size_t ,std::thread> m_clientThreads; ///Map of all agent ids and their client threads
        std::set<size_t> m_interface_set;   /// Set of interfaces on the server

        std::map<size_t, size_t> m_action_map; ///Map of agent IDs to most recent action selected

        std::mutex m_actionMapMutex; ///Mutex regarding the action map

        std::mutex m_connectionThreadMutex; ///Mutex regarding all connection threads

        std::string m_currentSerializedAgents; ///String with all current serialized agents

		std::vector<std::pair<sf::IpAddress, unsigned short>> m_updateVec;

    protected:

    public:
        const static constexpr unsigned short m_initConnectionPort = 55000; ///Port for initial client connection

        unsigned short m_maxClientPort = 55000; ///Port that is incremented for client thread handoff

        bool hasNewAgent;

        bool interfacesPresent = false;

        /**
         * Default constructor (AgentBase)
         * @param id agent ID
         * @param name agent name
         */
        ServerManager() = default;

        std::string GetSerializedAgents(){return m_currentSerializedAgents;}

        void SetSerializedAgents(std::string & serializedAgents) {m_currentSerializedAgents = serializedAgents;}


        /**
         * Convert action map to packet to send to client
         * @return packet containing action map as series of integers
         */
        sf::Packet ActionMapToPacket()
        {
            sf::Packet pkt;

            // serialize action map
            pkt << m_action_map.size();
            for (auto pair: m_action_map) {
                pkt << pair.first << pair.second;
            }

            return pkt;
        }

        std::mutex & GetThreadMutex(){return m_connectionThreadMutex;}

        std::mutex & GetActionMutex(){return m_actionMapMutex;}

        /**
         * Increases the max client port
         */
        void IncreasePort(){++m_maxClientPort;}

        void JoinClient(size_t id){
            m_interface_set.erase(id);
            if (m_interface_set.empty()) interfacesPresent = false;
        }

		void AddToUpdatePairs(sf::IpAddress ip, unsigned short port){
			m_updateVec.emplace_back(ip, port);
		}

		 void SendGameUpdates(){
			if (hasNewAgent)
			{
				sf::Packet serializedAgentPkt;
				serializedAgentPkt << GetSerializedAgents();
				for (auto client: m_updateVec){
					std::cout << "sending game updates to IP: " << client.first.toString() << " with port " <<
					client.second << std::endl;
					if (m_manager_socket.send(serializedAgentPkt, client.first, client.second) != sf::Socket::Status::Done) {
                        std::cerr << "Error sending updates to client at " << client.first.toString() << " port " << client.second << std::endl;
                    }
				}
				hasNewAgent = false;
			}
		}


        bool ActionMapContains(size_t key){return m_action_map.contains(key);}

        size_t ReadFromActionMap(size_t key){
            std::lock_guard<std::mutex> actionLock(m_actionMapMutex);
            try{
                return m_action_map.at(key);
            }

            catch (std::out_of_range & e){
                return 0;
            }
        }

        void RemoveFromActionMap(size_t key){
            m_action_map.erase(key);
        }

		void RemoveFromUpdatePairs(sf::IpAddress ip, unsigned short port){
			m_updateVec.erase(std::remove_if(m_updateVec.begin(), m_updateVec.end(),
				[ip, port](std::pair<sf::IpAddress, unsigned short> pair){
				return (pair.first == ip && pair.second == port);
			}), m_updateVec.end());
		}

        void WriteToActionMap(size_t key, size_t val){
            std::lock_guard<std::mutex> actionLock(m_actionMapMutex);
            m_action_map.insert_or_assign(key, val);
        }

        void AddToInterfaceSet(size_t agent_id){
            std::lock_guard<std::mutex> threadLock(m_connectionThreadMutex);
            m_interface_set.insert(agent_id);
            interfacesPresent = true;
        }

    }; // End of class ServerManager
} // End of namespace netWorth